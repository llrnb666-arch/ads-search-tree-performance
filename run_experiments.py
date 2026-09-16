"""Run reproducible experiments and generate CSV, Markdown, and SVG artifacts."""
from __future__ import annotations

import csv
import statistics
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BUILD = ROOT / "build"
EXE = BUILD / ("tree_benchmark.exe" if sys.platform == "win32" else "tree_benchmark")
SIZES = (100, 500, 1000, 2000)
REPEATS = 5
FIELDS = ("height", "comparisons", "node_visits", "rotations", "time_ms")


def build() -> None:
    subprocess.run(["cmake", "-S", str(ROOT), "-B", str(BUILD)], check=True)
    subprocess.run(["cmake", "--build", str(BUILD), "--config", "Release"], check=True)
    subprocess.run(["ctest", "--test-dir", str(BUILD), "-C", "Release", "--output-on-failure"], check=True)


def collect() -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for n in SIZES:
        for repeat in range(1, REPEATS + 1):
            output = subprocess.run([str(EXE), str(n)], check=True, text=True,
                                    capture_output=True).stdout.splitlines()
            for row in csv.DictReader(output):
                row["n"], row["repeat"] = str(n), str(repeat)
                rows.append(row)
    return rows


def summarize(rows: list[dict[str, str]]) -> list[dict[str, str]]:
    result: list[dict[str, str]] = []
    for n in SIZES:
        for workload in ("sorted", "random", "locality"):
            for tree in ("BST", "AVL", "Splay"):
                group = [r for r in rows if int(r["n"]) == n and
                         r["workload"].strip() == workload and r["tree"].strip() == tree]
                item = {"n": str(n), "workload": workload, "tree": tree}
                for field in FIELDS:
                    values = [float(r[field]) for r in group]
                    item[f"{field}_mean"] = f"{statistics.mean(values):.3f}"
                    item[f"{field}_stdev"] = f"{statistics.stdev(values):.3f}"
                result.append(item)
    return result


def write_csv(path: Path, rows: list[dict[str, str]]) -> None:
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=rows[0].keys())
        writer.writeheader(); writer.writerows(rows)


def make_svg(summary: list[dict[str, str]]) -> None:
    rows = [r for r in summary if r["workload"] == "sorted" and int(r["n"]) == max(SIZES)]
    colors = {"BST": "#ef4444", "AVL": "#22c55e", "Splay": "#3b82f6"}
    maximum = max(float(r["height_mean"]) for r in rows)
    parts = ['<svg xmlns="http://www.w3.org/2000/svg" width="760" height="420" viewBox="0 0 760 420">',
             '<rect width="100%" height="100%" fill="#111827"/>',
             '<text x="380" y="35" text-anchor="middle" fill="white" font-size="20">Sorted workload: final tree height (n=2000)</text>']
    for i, r in enumerate(rows):
        value = float(r["height_mean"]); height = 300 * value / maximum
        x = 120 + i * 220; y = 355 - height
        parts += [f'<rect x="{x}" y="{y:.1f}" width="120" height="{height:.1f}" fill="{colors[r["tree"]]}"/>',
                  f'<text x="{x+60}" y="{y-8:.1f}" text-anchor="middle" fill="white" font-size="16">{value:.0f}</text>',
                  f'<text x="{x+60}" y="385" text-anchor="middle" fill="white" font-size="17">{r["tree"]}</text>']
    parts.append('</svg>')
    (ROOT / "results" / "sorted-height.svg").write_text("\n".join(parts), encoding="utf-8")


def make_report(summary: list[dict[str, str]]) -> None:
    n = max(SIZES); chosen = [r for r in summary if int(r["n"]) == n]
    lines = ["# Experimental Report: BST vs AVL vs Splay", "",
             "## Method", "", f"Each workload was run {REPEATS} times for n = {', '.join(map(str, SIZES))}. "
             "All trees received exactly the same operation sequence. The random generator has a fixed seed.", "",
             "## Results at n = 2000", "", "| Workload | Tree | Height | Comparisons | Rotations | Time (ms) |",
             "|---|---:|---:|---:|---:|---:|"]
    for r in chosen:
        lines.append(f'| {r["workload"]} | {r["tree"]} | {float(r["height_mean"]):.1f} | '
                     f'{float(r["comparisons_mean"]):.0f} | {float(r["rotations_mean"]):.0f} | '
                     f'{float(r["time_ms_mean"]):.3f} |')
    lines += ["", "![Sorted input height](sorted-height.svg)", "", "## Interpretation", "",
              "- Sorted insertion exposes the unbalanced BST worst case: its height grows linearly and comparisons grow quadratically.",
              "- AVL keeps logarithmic height in every workload, paying a bounded number of rotations for predictable operations.",
              "- Splay may end an experiment with a tall tree and an individual operation may be linear; its guarantee concerns the amortized cost of the complete sequence.",
              "- Timing values this small depend on the machine and timer resolution. Structural counts are the primary evidence; timing is supporting evidence.",
              "", "## Reproducibility", "", "Run `python run_experiments.py` from the project directory. Raw observations are in `raw.csv`; means and sample standard deviations are in `summary.csv`."]
    (ROOT / "results" / "report.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> None:
    (ROOT / "results").mkdir(exist_ok=True)
    build(); raw = collect(); summary = summarize(raw)
    write_csv(ROOT / "results" / "raw.csv", raw)
    write_csv(ROOT / "results" / "summary.csv", summary)
    make_svg(summary); make_report(summary)
    print(f"Generated {len(raw)} observations in {ROOT / 'results'}")


if __name__ == "__main__":
    main()

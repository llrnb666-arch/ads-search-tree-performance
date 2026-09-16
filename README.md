# Research Project: BST vs AVL Tree vs Splay Tree

本项目在相同的插入、删除序列上比较三种搜索树：非平衡 BST、AVL Tree 和 Splay Tree。

## 当前实现

- 三种树均支持插入、删除、查找、树高、结点数、结构验证和内存释放。
- AVL Tree 维护结点高度并在插入、删除后重新平衡。
- Splay Tree 使用 Zig、Zig-Zig、Zig-Zag 对访问结点进行伸展。
- 统一记录 key 比较次数、访问结点数、旋转次数、最终树高和运行时间。
- 内置 `sorted`、`random`、`locality` 三类可复现实验。

## 构建与运行

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
./build/Release/tree_benchmark.exe 2000
```

单配置生成器的程序路径通常是 `./build/tree_benchmark`。参数 `2000` 是基础数据规模，可替换为其他大于等于 10 的整数。

输出为 CSV，可重定向到文件：

```powershell
./build/Release/tree_benchmark.exe 10000 > results.csv
```

## 指标说明

- `comparisons`：算法进行的 key 比较次数。
- `node_visits`：插入、删除或伸展过程中检查的结点数。
- `rotations`：平衡或伸展执行的单旋次数；普通 BST 恒为 0。
- `height`：实验完成后的树高，空树为 0、单结点树为 1。
- `time_ms`：完整操作序列的 CPU 时间。短实验容易受计时精度影响，应扩大 n 并重复实验。

这些“基本操作次数”比单次运行时间更适合解释算法差异；运行时间则反映当前机器和编译器下的实际表现。

## 下一阶段

运行完整、可复现的实验并生成原始 CSV、统计汇总、SVG 图表和 Markdown 报告：

```powershell
python run_experiments.py
```

生成结果位于 `results/`。默认测试 4 种规模，每组重复 5 次并报告样本标准差。

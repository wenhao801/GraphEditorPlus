## Graph Editor Plus

Inspired by [CSAcademy's graph editor](https://csacademy.com/app/graph_editor/), [Graphviz](https://graphviz.org/) and others.

### 基础功能

实现加点、加边功能，完成无向图/有向图的绘制（含/不含边权，不含边权的图内部实现为边权置1）。具体途径比如

- 通过鼠标点击，绘制单个点/边。
- 批量修改：
  - 通过文本批量加点/加边，类似于 [CSAcademy's graph editor](https://csacademy.com/app/graph_editor/) 中的方式。
  - 可以加入一些小功能，例如输入 `[3-9]` 批量加入编号为 `3, 4, ..., 9` 的点。
  - 通过框选，将框中的点直接互相连成链/完全图/……
- 批量修改点编号的功能，类似于几何画板。

实现删点、删边功能，例如

- 通过鼠标点击，删除单个点/边。
- 批量修改：
  - 直接编辑当前图对应的文本，进行修改。
  - 通过框选，批量对与选择框相交的边进行删除。

实现图上点、边可以拖拽。

### 可能会有的附加功能（简单）

直接对图上跑算法，例如最短路、最小生成树、判环、拓扑排序等。

对点、边的个性化设置（如更改颜色、样式、……）

### 可能会有的附加功能（中等）

实现图上点、边可以缩放。

新功能：利用已经写好的界面，实现数据结构（堆、搜索树、……）的可视化。

### 可能会有的附加功能（困难）

实现图上比较小的点、边可以聚集显示。

新功能：神经网络计算图模拟。

加入类似于 [CSAcademy's graph editor](https://csacademy.com/app/graph_editor/) 中 unfix 模式的物理系统，让图上的点、边可以自动地间隔开。// 找到了参考代码（大概），好像也不是那么难。

### 程序结构

```cpp
struct Node {
    string name;
    set <Edge*> in, out; // 可能有按地址找边/按指向的点找边，因此可能之后再写数据结构
    // 无向图时 in === out
};
```

```cpp
struct Edge {
    Node *u, *v;
    string w; // weight
    Node* ad(Node* now) { return u == now ? v : u; }
};
```

## ChangeLog

搭了一个自定义的 MyNode 类。

TODO： 点在叠放时的上下层问题？（现在好像是：后生成的点总在上面，希望改为：将每次拖动的点的层级设置为最高）

TODO：拖动空白区域时，Scene 自动扩展（可能需要自定义 View，再重载相关的 event）

TODO：删点

TODO：将点拖出边框后边 会 断 掉，拖得太快有残影？

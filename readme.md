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

### 可能会有的附加功能（简单）

直接对图上跑算法，例如最短路、最小生成树、

阿萨范德萨发撒地方 萨芬撒地方
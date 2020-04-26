# Grafana配置

- https://kiswo.com/article/1021

## 基本概念

- **Data Source**: 各种DB的数据；
- **Organization**: 用户、数据源和仪表板都隶属于组织。用户隶属于某个组织后。就拥有该组织下的所有数据源和仪表板；
- **User**：用户可以属于一个或多个组织，并可以通过角色分配不同级别的权限；
- **Role**：角色决定权限。默认是`viewer`。
- **Dashboard**：由多个panel组成。仪表板可以看做为一个展示单元；
- **Row**：Dashboard里的逻辑分格符，将各个panel组合在一起。一个Row可以有多个panel；
- **Panel**：数据直接展示在Panel上；
  - Graph：多指标|系列；
  - Singlestat：只查询单个数字；
  - Dashlist：不连接到任何数据源的特殊面板；
  - Table
  - Text：不连接到任何数据源的特殊面板；
  - 可以通过插件安装的方式来扩展面板类型。例如：`Pie Chart`、`Worldmap Panel`。
- **Query Editor**：用来显示数据源，并根据数据源查询其包含的指标。

> 将鼠标放在面板的`Panel Title`位置，然后单击，就会显示出查询编辑，点击 `Edit`，就显示出查询编辑器了。

- **Templating**：Template of dash board.




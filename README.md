## 项目说明  

* __简介：__ 该项目为科研项目，由三人合作完成，本人负责将爬取到的城市热数据导入到UE中进行呈现，在UE中的所有工作都由本人完成。

* __主要实现思路：__ 爬取到的数据格式为csv格式，坐标系为WGS84经纬度坐标。在UE中设置好data structure后可以直接导入成表格的形式，再通过Cesium for Unreal插件将经纬度坐标转换到虚幻引擎坐标系。再根据温度信息生成不同颜色的小球，反映相应位置的热舒适度信息。此外还提供多种方式对数据进行查询和交互。

* __涉及到的功能模块：__ C++/蓝图、摄像机控制、UMG模块的设计与交互、利用深度测试提取边线、Niagara粒子系统

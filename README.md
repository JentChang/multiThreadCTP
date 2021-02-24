# 多线程的ctp 

### 依赖环境
vs2017

cmake 3.19

git subtree add --prefix=lib/rapidjson   https://github.com/Tencent/rapidjson.git      v1.1.0   --squash

## 使用
在 build/data.json/ctp_config/ctp_config.json 中设置你的账户信息
### 添加策略

* step1

	strategy.cpp文件夹内创建文件
	
	命名Strategy_XXXXX.h Strategy_XXXXX.cpp（拷贝例子更改）

* step2

	built 执行cmake
	
* step3

	vs打开工程 在strategy_cpp中
	
	Strategy_XXXXX.cpp ReceiveTick or OnBar中实现自己的策略

* step4

	cpp 文件中Run.cpp include Strategy_XXXXX.h文件

	Run.cpp 中注册策略对象
	
	`StrategyTemplate* st = new Strategy_rb_rsi(tdevent);`

	`mdevent->AddStrategy(st)`

-------------

vs中编译，设置cpp为启动项目 bebug/release x64运行

	


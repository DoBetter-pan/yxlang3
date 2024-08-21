1、说明
本文档是说明如果在引擎工程中使用yxlang语言的。
2、Context类
该类定义在context.h。该类可以自定义请求上下文信息，最后会以userdata的方式传入到httphandler中。用于传递请求与返回的内容。
3、CHttpHandlerNode类
该类定义在httphandler.h中。该类是代码最终执行的地方，可以自定义函数的行为, 根据具体的需要来实现对应的代码。
4、CYxHelper类
该类定义在yxhelper.h、yxhelper.cc中。用于连接yxlang语言的外部与内部调用的。
5、使用：
1）在CYxHelper类中加载对应的脚本文件 conf/flow.yx , 见下面

a = http_create()
if http_handle(a, 2) == 1 then http_handle(a, 6); fi
if http_handle(a, 5) == 1 then http_handle(a, 7); else http_handle(a, 8); http_handle(a, 9); fi
http_handle(a, 1)
http_handle(a, 2)
http_destory(a)

2）每个请求都需要调用http_create创建对应的句柄，然后通过句柄就可以调用对应的handle函数，最后通过http_destory销毁句柄。
3）http_handle最终会调用CHttpHandlerNode::Handle函数。
4）a[uri]最终会调用CHttpHandlerNode::Get函数。
5）http_async最终会调用CHttpHandlerNode::Async函数。该函数实现了一个简单的协程例子，具体引擎可以根据具体的情况实现异步线城处理。
6）http_wait最终会调用CHttpHandlerNode::Wait函数。该函数会等待信号量之后才会继续执行，请注意释放对应的信号量。

注：目前不支持字符串的字符量。



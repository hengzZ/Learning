## 表单提交

##### 前端与后台 server 的数据交互方式
* Form 表单提交
* Ajax 请求
* http 请求
* cookie/session

<br>

#### 1. Form 表单提交
html 的 \<form> 标签定义了表单，表单可以包含不同类型的子元素，通过设置 \<input> 子元素的 type 特性来指定，或使用 \<a> \<select> \<textarea> \<button> 子元素等。

##### 1.1 Form 的 Action 属性
action 属性指定了表单提交后执行的操作，例如发送地址。 如果省略了 action 属性，则默认的 action 是发送到当前网页。

##### 1.2 Form 的 Target 属性
target 属性指定提交的结果是否在新的 browser tab、frame 或 current window 中打开。 默认是 ``_self``，表示表单在当前窗口中提交。
``_blank`` 表示在 new browser tab 打开。

##### 1.3 Form 的 Method 属性
method 属性指定了提交表单数据时使用的 http 方法。 支持两种方法 GET or POST。 缺省是使用 GET 方法。
* 使用 GET 方法时，表单数据将显示在页面地址字段中。 形如 ``/action_page.php?firstname=Mickey&lastname=Mouse``。 主要用于非敏感数据。
* POST 方法主要用于敏感和个人数据，没有长度限制能够发送大量的数据，不能被 bookmarked。（GET 方法可以被 bookmarked）

注意，
* 每一个 \<input> 子元素必须设置 name 属性。 如果省略了 name 属性，则表单提交时此 input 字段不被发送。

<br>

#### 2. Ajax
*Perform an asynchronous HTTP (Ajax) request.*

##### 2.1 jQuery 的 ajax 语法
```html
jQuery.ajax( url [, settings ] )
```
* url 指定 http request 发送到的地址。
* settings 是 A set of key/value pairs that configure the Ajax request. (optional)

<br>

#### 3. Form、Ajax 和 Angular $http 的区别
##### 3.1 提交方式的区别
* Form 通过在 html 定义的 action、method、submit 来提交，也可通过 js 中调用 submit 函数来提交，具体的提交方式有很多种。
* Ajax 和 $http 都是基于 XMLHttpRequest 进行提交。

##### 3.2 页面刷新
* Form 提交更新数据完成后，需要转到一个空白页，再对原页面进行提交后处理。 哪怕是提交到当前页面，也需要整页刷新。
* Ajax、$http 都可以实现页面的局部刷新。

##### 3.3 请求是由谁来提交的？
* Form 提交是由浏览器完成，无论浏览器是否开启 js 支持，都可完成。
* Ajax、$http 是通过 js 来提交请求，请求和响应均由 js 引擎来处理。

##### 3.4 是否可上传文件
* Ajax 出于安全性考虑，不能对文件进行操作。 可通过隐藏的 Form 提交来实现需求。
* 后来 XMLHttpRequest 引入 FormData 类型，使 Ajax 可实现文件上传。

<br>

#### 4. Ajax v.s. $http
##### 4.1 默认 content-type 类型
* Ajax 默认的 Content-type 是 x-www-form-urlencoded。
* $http 默认的 Content-type 是 application/json。

##### 4.2 async
* Ajax 支持同步通信(async:false)。
* $http 不支持 async:false。

##### 4.3 参数处理
* Ajax 在 post 数据之前，jQuery 会对数据进行序列化，转换成字符串 "a=1&b=2" 形式。
* $http 不会对数据做参数处理，数据将以 json 格式发送。

<br>

#### 5. http POST 提交数据的四种解析方式
HTTP 协议是以 ASCII 码传输，建立在 TCP/IP 协议之上，其中 POST 一般用来向服务端提交数据。

##### 5.1 协议规定
* 协议规定 POST 提交的数据必须放在消息主体（entity-body）中，但并未规定数据必须使用什么编码方式。
* 服务端通常是根据请求头（headers）中的 Content-Type 字段来获知请求中的消息主体是用何种方式编码。
* 在服务器端判断 request 来自 Ajax 请求(异步)还是传统请求(同步)。 （通过 headers 中的字段）

规范把 HTTP 请求分为三个部分：状态行、请求头、消息主体。类似于下面这样：
```html
<method> <request-URL> <version> <headers> <entity-body>
```

##### 5.2 四种编码方式
* application/x-www-form-urlencoded （默认常用的）
* multipart/form-data
* application/json
* text/xml （现在几乎不用）

##### 5.5 GET 方法提交数据的编码方式
GET 方式的数据提交方式（编码方式）只有一种，就是 application/x-www-form-urlencoding。

<br>

#### 6. MIME
"Multipurpose Internet Mail Extensions" 多功能 Internet 邮件扩充服务，它是一种多用途网际邮件扩充协议。

服务器会将它们发送的多媒体数据的类型告诉浏览器，而通知手段就是说明该多媒体的 MIME 类型。
```html
text	表明文件是普通文本，理论上是可读的语言。
        示例： text/plain, text/html, text/css, text/javascript
image	表明是某种图像。不包括视频，但是动态图（比如动态gif）也是 image 类型。
        示例： image/gif, image/png, image/jpeg, image/bmp, image/webp
audio	表明是某种音频文件。
        示例： audio/midi, audio/mpeg, audio/webm, audio/ogg, audio/wav
video	表明是某种视频文件。
        示例： video/webm, video/ogg
application	表明是某种二进制数据
        示例： application/octet-stream, application/pkcs12, application/vnd.mspowerpoint,
              application/xhtml+xml, application/xml,  application/pdf, application/json
```
* 对于 text 文件类型若没有特定的 subtype，就使用 text/plain。
* 类似的，二进制文件没有特定或已知的 subtype，就使用 application/octet-stream。

<br>

#### 7. cookie 与 session 的区别
##### 7.1 存放差异
* cookie 以文本格式存储在浏览器上，存储量有限。
* session 存储在服务端，可以无限量存储多个变量，并且比 cookie 更安全。

##### 7.2 用途不同
—— 两者结合弥补 http 的无状态特征。
* Cookie 弥补了 HTTP 无状态的不足，让服务器知道来的人是 “谁”。 （无状态的意思是每次请求都是独立的。）
* 通过 Cookie 识别不同的用户，对应的在 Session 里保存私密的信息。

##### 7.3 生命周期
* 服务器会把长时间没有活动的 Session 从服务器内存中清除，此时 Session 便失效。 Tomcat 中 Session 的默认失效时间为 30 分钟。
* setMaxAge() 方法便可以设置 Cookie 对象的有效时间。 如果不设置过期时间，则表示只要关闭浏览器窗口，cookie 就消失了（此时称为 会话 Cookie）。

注意，
* 新开的浏览器窗口会生成新的 Session，但子窗口除外。 子窗口会共用父窗口的 Session。
* 如果客户端浏览器将 Cookie 功能禁用，或者不支持 Cookie 怎么办？ Java Web 提供了另一种解决方案：URL 地址重写。
* 会话 cookie 一般不保存在硬盘上而是保存在内存里。 非会话 cookie 存储在硬盘上，在有效期内可在不同的浏览器进程间共享。

## Web Media Player

##### 网页媒体播放
* Animate
* Video/Audio

<br>

#### 1. Web 动画的一些实现方式
##### 1.1 GIF 动画
GIF 是将多幅图像保存为一个图像文件，归根到底 GIF 仍然是图片文件格式。

优缺点：
* 高清的 gif 体积较大，压缩后会失真。
* 交互差，因为实际上就是一张图片。
* 可通过 PS 和一些制作软件制作，支持性好。

##### 1.2 FLASH 动画 / SilverLight
Flash 非常强大，利用 Flash 可以制作各种各样非常华丽的动画和视频。
SilverLight 是微软的一个跨浏览器、跨平台插件，是一种新的 Web 呈现技术，可以使用在浏览器内和桌面系统中。（知道即可）

优缺点：
* 这种技术逐渐淡出了视野，目前只有视频网站还在使用。
* 基于 flash 播放器来播放，flash 播放器非常耗性能。

##### 1.3 Javascript + HTML
主要思想是通过 setInterval 或 setTimeout 方法的回调函数，持续调用改变某个元素的 CSS 样式以达到元素样式变化的效果。
JQuery 的 animate() 方法就是这种方式实现的。

优缺点：
* 会导致页面频繁性重排重绘，消耗性能，一般应用在桌面端浏览器。
* 在移动端上使用会有明显的卡顿。

##### 1.4 Javascript + SVG
SVG 指可伸缩矢量图形 (Scalable Vector Graphics)

##### 1.5 Javascript + Canvas
Canvas 作为 H5 新增元素，是借助 Web API 来实现动画的。 结合 setInterval 或者 requestAnimationFrame 可以实现各种样的动画。

优缺点：
* Canvas 有些复杂。
* Canvas 主要优势是可以应对页面中多个动画元素渲染较慢的情况。
* 完全通过 javascript 来渲染控制动画的执行，可用于实现较复杂动画。

##### 1.6 CSS3 transition/animation
transition 是过渡动画，transition 并不能实现独立的动画。
animation 算是真正意义上的 CSS3 动画，通过对关键帧和循环次数的控制。

优缺点：
* 在移动端开发中，直接使用 transition 动画会让页面变慢甚至卡顿，通常添加 transform:translate3D(0,0,0) 或 transform:translateZ(0) 来开启移动端动画的 GPU 加速。
* CSS3 最大的优势是摆脱了 js 的控制，并且能利用硬件加速以及实现复杂动画效果。

##### 补充说明
* 以上技术并不存在哪个更厉害，而是优势和适用场景不同。
* CSS 动画可以对动画优化，但只能做简单的动画，有很多限制。 （不能中间反转动画、不能控制时间灵活度等）
* Flash 动画需要浏览器插件支持，否则出现播放问题。
* Javascript 脚本动画能力很强，可以完成很多的动画动作，缺点是较为复杂。
* PC 端动画建议使用 Javascript，移动端建议使用 css+html5。

<br>

#### 2. Web 应用的 音/视频 播放
—— 需要借助插件(Plugin)来实现。
##### 2.1 H5 的 Video 新标签
仅支持三种视频格式： Ogg(Theora 视频编码 + Vorbis 音频编码)、 MPEG 4(H.264 视频编码 + AAC 音频编码)、 WebM（VP8 视频编码 + Vorbis 音频编码）。

##### 2.2 JW Player
—— https://www.jwplayer.com

JW Player 是一个在网页上使用的 Flash 播放器。

##### 2.3 JQuery 的 video.js
用量较多，但是移动端的兼容性不好。

##### 2.4 CND 云
视频是互联网中最耗带宽的传输形式，一台服务器能够承受的访问量、能够提供支撑的带宽是有限的。 视频网站必须使用 CDN 对视频服务器进行分流减负。

视频 CDN 一般是由缓存服务器与内容分发服务器两种服务器构成。

##### 补充说明
* 不同的 web 框架/库 有对应的视频播放插件，例如 JQuery、React、Vue 兼容的插件。
* 难点： 对于较大的视频文件，需要支持边缓冲边播放。

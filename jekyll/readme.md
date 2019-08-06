## jekyll
—— https://jekyllrb.com

*Transform your plain text into static websites and blogs.*

##### 特点
* Simple — 不需要数据库，没有评论审核，没有讨厌的安装更新。
* Static — 仅支持静态网页。 仅支持 Markdown 和 HTML 两种文本，支持 Liquid 模板语言，支持 HTML+CSS。
* Blog-aware — 博客相关的特性，永久链接、分类、页面、发帖和自定义布局都有。

##### 介绍
jekyll 是一个基于 ruby 开发的，用于构建静态网站的程序。 其作用是将特定格式的文本转换为 html 文件（静态网页），它还包含一整套的 web server 功能。
部署了 jekyll 运行环境，我们就可以使用浏览器查看自己的文档（当然是以网页形式展现出来）。

<br>

### Part 1 —— jekyll 部署
#### 1 常用命令
```bash
jekyll new # 创建一个新的网站。 （即一个新的项目 project）
jekyll build # 生成，将文档转为网站网页，用于生产部署前。
jekyll serve # 执行 build 并且 serves it locally。 用于开发时的测试和预览。
jekyll clean # 清除，清除生成网页的结果。

jekyll doctor # 输出 issues，潜在问题检查。
jekyll help # 帮助
```

#### 2 jekyll 安装
##### 2.1 预备知识 —— Ruby Gem
RubyGems 是 Ruby 的一个包管理工具，类似于 node.js 的 npm 以及 python 的 pip。

##### 2.2 installation
—— https://jekyllrb.com/docs/installation

* Windows 版安装
```bash
# 1. 下载安装 RubyInstaller - https://rubyinstaller.org/downloads/
#    选择 Ruby+Devkit 版本，安装时 使用默认选项即可。
# 注意， 安装的最后一步要勾选 Run 'ridk install' to 选项，然后选择 base installation 即可。
# 2. 使用 gem 安装 ruby 依赖包
gem install jekyll bundler
jekyll -v
```
* gem 指定代理 or 更换为 淘宝源
```bash
# 设置独立代理
gem install -p http://proxy-url:port jekyll bundler
或
gem install -p https://proxy-url:port jekyll bundler
# 更换源
gem sources -l 
gem sources --remove https://rubygems.org/
gem sources -a https://ruby.taobao.org/
```

##### 2.3 运行环境测试
```bash
jekyll -v
```

<br>

### Part 2 —— 发布内容
##### 1. QuickStart
```bash
jekyll new myblog
cd myblog
bundle exec jekyll serve
# Now browse to http://localhost:4000
```
或者
```bash
jekyll new --skip-bundle myblog
cd myblog
jekyll serve
```

如果报错，提示未找到 ``'minima (~> 2.0) x64-mingw32``，执行
```bash
gem install minima # 如果需要， -p https://proxy-url:port 添加代理
```
其他的依赖缺失，同样方法处理。 注意，在重新执行 ``jekyll serve`` 前，删除 ``Gemfile`` 文件。

##### 2. Step-by-Step Tutorial
—— https://jekyllrb.com/docs/step-by-step/01-setup/
* Setup 环境部署
* Liquid 模板语言语法 （核心）
* Front Matter 文件内容的前几行 （核心）
* Layouts 布局/模板/式样 （核心）
* Includes 导航 （核心）
* Data Files 纯内容数据的加载 （超核心） *Jekyll supports loading data from YAML, JSON, and CSV files located in a _data directory.*
* Assets 使用 CSS, JS, images 和其他 assets （核心）
* Blogging 博客的文件结构 （如果需要博客系统的话）
* Collections 隔离每个作者的博客内容 （多个作者，每个有自己的博客列表页）
* Deployment 生产环境/发布部署

<br>

#### ▪ Other Topics
##### 1. 为什么 web 框架/程序 都会支持一种模板语言？
* 模板对于 web 页面相当于 PPT 的主题。
* 模板定义了页面的布局和式样，内容只不过是式样上覆盖（悬浮）的文字/图片。
* 模板使主题更换更容易。

##### 2. 其他替代方案
* **WordPress** — 基于 PHP， https://cn.wordpress.org/
* **hexo** — 基于 Node.js， https://hexo.io/

说明
* 三种项目的思路相似，不同点在于： 文本格式支持、模板语言语法和项目组织方式。

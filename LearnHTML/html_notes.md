# Getting started with HTML
reference: https://www.w3.org/MarkUp/Guide/ <br>
**What is HTML?** <br>
It is a special kind of text document that is used by Web browsers to present text and graphics. <br>
The text includes **markup tags** such as \<p> to indicate the start of a paragraph, and \</p> to indicate the end of a paragraph. <br>
**Advanced feature** <br>
Many people still write HTML by hand using tools such as NotePad on Windows, or TextEdit on the Mac. This guide will get you up and running. Even if you don't intend to edit HTML directly and instead plan to use an HTML editor such as Netscape Composer, or W3C's Amaya, this guide will enable you to understand enough to make better use of such tools and how to make your HTML documents accessible on a wide range of browsers. Once you are comfortable with the basics of authoring HTML, you may want to learn how to [add a touch of style](https://www.w3.org/MarkUp/Guide/Style.html) using CSS, and to go on to try out features covered in my page on [advanced HTML](https://www.w3.org/MarkUp/Guide/Advanced.html)


## Start with a title
Every HTML document needs a title. Here is what you need to type:
```html
<title>My first HTML document</title>
```
The title should be placed at the beginning of your document.


## Add headings and paragraphs
If you have used Microsoft Word, you will be familiar with the built in styles for headings of differing importance. In HTML there are six levels of headings. H1 is the most important, H2 is slightly less important, and so on down to H6:
```html
<h1>An important heading</h1>
<h2>A slightly less important heading</h2>
```
Each paragraph you write should start with a \<p> tag. The \</p> is optional:
```html
<p>This is the first paragraph.</p>
<p>This is the second paragraph.</p>
```


## Adding a bit of emphasis
You can emphasize one or more words with the \<em> tag:
```html
This is a really <em>interesting</em> topic!
```


## Adding interest to your pages with images
The simple way to add an image is using the \<img> tag. Let's assume you have an image file called "peter.jpg" in the same folder/directory as your HTML file. It is 200 pixels wide by 150 pixels high.
```html
<img src="peter.jpg" width="200" height="150">
```
The width and height aren't strictly necessary but help to *speed the display* of your Web page. 

**Something is still missing! People who can't see the image need a description they can read in its absence.**
```html
<img src="peter.jpg" width="200" height="150"
alt="My friend Peter">
```

**For complex images, you may need to also give a longer description. Assuming this has been written in the file "peter.html", you can add one as follows using the longdesc attribute:**
```html
<img src="peter.jpg" width="200" height="150"
alt="My friend Peter" longdesc="peter.html">
```


## Adding links to other pages
**What makes the Web so effective** is the ability to define links from one page to another, and to follow links at the click of a button. A single click can take you right across the world!
Links are defined with the \<a> tag:
```html
This a link to <a href="peter.html">Peter's page</a>.
```
The text between the \<a> and the \</a> is used as the *caption* for the link. It is common for the caption to be in blue underlined text.

The use of *relative paths* allows you to link to a file by walking up and down the tree of directories as needed, for instance:
```html
<a href="../college/friends/john.html">John's page</a>
```

**You can turn an image into a hypertext link**, for example, the following allows you to click on the company logo to get to the home page:
```html
<a href="/"><img src="logo.gif" alt="home page"></a>
```


## Three kinds of lists
HTML supports three kinds of lists. The first kind is a bulletted list, often called an *unordered list*. It uses the \<ul> and \<li> tags, for instance:
```html
<ul>
  <li>the first list item</li>

  <li>the second list item</li>

  <li>the third list item</li>
</ul>
```

The second kind of list is a numbered list, often called an *ordered list*. It uses the \<ol> and \<li> tags. For instance:
```html
<ol>
  <li>the first list item</li>

  <li>the second list item</li>

  <li>the third list item</li>
</ol>
```

The third and final kind of list is the *definition list*. This allows you to list terms and their definitions. This kind of list starts with a \<dl> tag and ends with \</dl> Each term starts with a \<dt> tag and each definition starts with a \<dd>. For instance:
```html
<dl>
  <dt>the first term</dt>
  <dd>its definition</dd>

  <dt>the second term</dt>
  <dd>its definition</dd>

  <dt>the third term</dt>
  <dd>its definition</dd>
</dl>
```

**Note that lists can be nested**, one within another. For instance:
```html
## ordered list
<ol>
  <li>the first list item</li>

  <li>
    the second list item
    ## unordered list
    <ul>
      <li>first nested item</li>
      <li>second nested item</li>
    </ul>
  </li>

  <li>the third list item</li>
</ol>
```


## HTML has a head and a body
**The structure of HTML pages** <br>
The document generally starts with a declaration of which version of HTML has been used, and is then followed by an \<html> tag followed by \<head> and at the very end by \</html>. The \<html> ... \</html> acts like a *container* for the document. The \<head> ... \</head> contains *the title, and information on style sheets and scripts*, while the \<body> ... \</body> contains *the markup with the visible content*. Here is a template you can copy and paste into your text editor for creating your own pages:
```html
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <title> replace with your document's title </title>
</head>
<body>

replace with your document's content

</body>
</html>
```


## Tidying up your markup
A convenient way to automatically fix markup errors is to use [HTML Tidy](https://www.w3.org/People/Raggett/tidy/) which also tidies the markup making it easier to read and easier to edit. I recommend you regularly run Tidy over any markup you are editing. Tidy is very effective at cleaning up markup created by authoring tools with sloppy habits. Tidy is available for a wide range of operating systems from the [TidyLib Sourceforge site](http://tidy.sourceforge.net/#binaries), and has also been integrated into a variety of HTML editing tools.

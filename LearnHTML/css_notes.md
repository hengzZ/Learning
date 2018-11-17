# Adding a touch of style
This is a short guide to styling your Web pages. It will show you how to use W3C's *[Cascading Style Sheets language](https://www.w3.org/Style/CSS/) (CSS)*. The route will steer you clear of most of the problems caused by differences between different brands and versions of browsers. <br>
**For style sheets to work, it is important that your markup be free of errors:** <br>
A convenient way to automatically fix markup errors is to use the [HTML Tidy](https://www.w3.org/People/Raggett/tidy/) utility. This also tidies the markup making it easier to read and easier to edit. I recommend you regularly run Tidy over any markup you are editing.


## Getting started
Let's start with setting the color of the text and the background. You can do this by using the **STYLE** element to set style properties for the document's tags:
```html
<style type="text/css">
  body { color: black; background: white; }
</style>
```
The style element is placed within the document head. Here is a template HTML file showing where the above style element fits.
```html
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title> replace with your document's title </title>
<style type="text/css">
  body { color: black; background: white; }
</style>
</head>
<body>

replace with your document's content

</body>
</html>
```
The stuff between the \<style> and \</style> is written in special notation for style rules. Each rule starts with a **tag** name followed by *a list of style properties* bracketed by **{** and **}**.


## Linking to a separate style sheet
If you are likely to want to use the same styles for several Web pages it is worth considering using a separate style sheet which you then link from each page. You can do this as follows:
```html
<link type="text/css" rel="stylesheet" href="style.css">
```
The **link** tag should be placed within the \<head> ... \</head> element.
```html
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title> replace with your document's title </title>
<link type="text/css" rel="stylesheet" href="style.css">
</head>
<body>

replace with your document's content

</body>
</html>
```
The link element's **rel** (relative) attribute must be set to the value "stylesheet" to allow the browser to recognize that the **href** attribute gives the Web address (URL) for your style sheet. *A simple stylesheet file might look like the following*:
```css
/* style.css - a simple style sheet */
body {
  margin-left: 10%; margin-right: 10%;
  color: black; background: white;
}
```
*Note that the same HTML file can link to an external style sheet and also include a style element for additional style settings specific to this page.* If you place the link element before the style element, you can use the latter to override the style settings in the linked style sheet.


## Setting the page margins
**Web pages look a lot nicer with bigger margins.** You can set the left and right margins with the "margin-left" and "margin-right" properties, e.g.
```html
<style type="text/css">
  body { margin-left: 10%; margin-right: 10%; }
</style>
```


## Setting left and right indents
**To make headings a little more distinctive**, you can make them start within the margin set for the body, e.g.
```html
<style type="text/css">
  body { margin-left: 10%; margin-right: 10%; }
  h1 { margin-left: -8%;}
  h2,h3,h4,h5,h6 { margin-left: -4%; }
</style>
```


## Controlling the white space above and below
Browsers do a pretty good job for the white space above and below headings and paragraphs etc. Two reasons for taking control of this yourself are: *when you want a lot of white space before a particular heading or paragraph*, or *when you need precise control for the general spacings*.

The "margin-top" property specifies the space above and the "margin-bottom" specifies the space below. To set these for all h2 headings you can write:
```html
h2 { margin-top: 8em; margin-bottom: 3em; }
```
The em is a very useful unit as it scales with the size of the font. **One em is the height of the font**. By using em's you can preserve the general look of the Web page independently of the font size. This is much safer than alternatives such as pixels or points, which can cause problems for users who need large fonts to read the text.

Note: Points are commonly used in word processing packages, e.g. 10pt text. Unfortunately the same point size is rendered differently on different browsers. What works fine for one browser will be illegible on another! Sticking with em's avoids these problems.

To specify the space above a particular heading, you should create a named style for the heading. You do this with the **class** attribute in the markup, e.g.
```html
<h2 class="subsection">Getting started</h2>
```
The style rule is then written as:
```html
h2.subsection { margin-top: 8em; margin-bottom: 3em; }
```
The rule starts with the tag name, a dot and then the value of the class attribute. *There are other ways to set the styles for a particular element but the class attribute is the most flexible.*


## First-line indent
```html
p { text-indent: 2em; margin-top: 0; margin-bottom: 0; }
```
It indents the first line of each paragraph by 2 em's and suppresses the inter-paragraph spacing.


## Controlling the font
This section explains how to set the font and size, and how to add *italic*, **bold** and other styles.

#### Font styles
The most common styles are to place text in italic or bold. Most browsers render the **em** tag in italic and the **strong** tag in bold. *Now, Let's assume you instead want em to appear in bold italic and strong in bold uppercase:*
```html
em { font-style: italic; font-weight: bold; }
strong { text-transform: uppercase;  font-weight: bold; }
```
If you feel so inclined, you can fold headings to lower case as follows:
```html
h2 { text-transform: lowercase; }
```

#### Setting the font size
You are recommended to specify font sizes in relative terms. This example sets heading sizes in percentages relative to the size used for normal text:
```html
h1 { font-size: 200%; }
h2 { font-size: 150%; }
h3 { font-size: 100%; }
```

#### Setting the font family
**It is likely that your favorite font won't be available on all browsers.** To get around this, you are allowed to list several fonts in preference order. There is a short list of generic font names which are guaranteed to be available, so you are recommended to end your list with one of these: *serif*, *sans-serif*, *cursive*, *fantasy*, or *monospace*, for instance:
```html
body { font-family: Verdana, sans-serif; }
h1,h2 { font-family: Garamond, "Times New Roman", serif; }
```

#### Avoid problems with fonts and margins
* My first rule is to avoid text at the body level that isn't wrapped in a block level element such as **p**. For instance:
```html
# wrong
<h2>Spring in Wiltshire</h2>

Blossom on the trees, bird song and the sound of lambs
bleating in the fields.
```
```html
# right
<h2>Spring in Wiltshire</h2>

<p>Blossom on the trees, bird song and the sound of lambs
bleating in the fields.</p>
```
* My second rule is to set the font family for *pre* elements, as some browsers forget to use a fixed pitch font when you set the font size or other properties for pre.
```html
pre { font-family: monospace; }
```
* My third rule is to set the font family on *headings*, *p* and *ul* elements if you intend to set borders or backgrounds on elements such as div. This is a work-around for a bug where the browser forgets to use the inherited font family, instead switching to the default font as set by the browser preferences.
```html
h1,h2,h3,h4,h5,p,ul { font-family: sans-serif; }
```


## Adding borders and backgrounds
You can easily add a border around a heading, list, paragraph or a group of these enclosed with a **div** (division) element. For instance:
```html
div.box { border: solid; border-width: thin; width: 100%; }
```
Note that without the "width" property some browsers will place the right margin too far to the right. This can then be used with markup such as:
```html
<div class="box">
The content within this DIV element will be enclosed
in a box with a thin line around it.
</div>
```
There are a limited choice of border types: *dotted*, *dashed*, *solid*, *double*, *groove*, *ridge*, *inset* and *outset*. The border-width property sets the width. Its values include *thin*, *medium* and *thick* as well as a *specified width* e.g. 0.1em. The border-color property allows you to set the color.

A nice effect is to paint the background of the box with a solid color or with a tiled image. To do this you use the background property. You can fill the box enclosing a div as follows:
```html
div.color {
    background: rgb(204,204,255);
    padding: 0.5em;
    border: none;
}
```

Without an explicit definition for border property some browsers will only paint the background color under each character. The padding property introduces some space between the edges of the colored region and the text it contains.

You can set different values for padding on the left, top, right and bottom sides with the *padding-left*, *padding-top*, *padding-right* and *padding-bottom* properties, e.g. padding-left: 1em.

Suppose you only want borders on some of the sides. You can control the border properties for each of the sides independently using the *border-left*, *border-top*, *border-right* and *border-bottom* family of properties together with the **appropriate suffix**: *style*, *width* or *color*, e.g.
```html
p.changed {
    padding-left: 0.2em;
    border-left: solid;
    border-right: none;
    border-top: none;
    border-bottom: none;
    border-left-width: thin;
    border-color: red;
}
```
which sets a red border down the left hand side only of any paragraph with the class "changed".


## Setting Colors
Some examples for setting colors appeared in earlier sections. Here is a reminder:
```html
body {
    color: black;
    background: white;
}
strong { color: red; }
```

#### Setting Link Colors
You can use CSS to set the color for hypertext links, with a different color for links that you have yet to follow, ones you have followed, and the active color for when the link is being clicked. You can even set the color for when the mouse pointer is hovering over the link.
```html
:link { color: rgb(0, 0, 153); }  /* for unvisited links */
:visited { color: rgb(153, 0, 153); } /* for visited links */
a:active { color: rgb(255, 0, 102); } /* when link is clicked */
a:hover { color: rgb(0, 96, 255); } /* when mouse is over link */
```
Sometimes you may want to show hypertext links without them being underlined. You can do this by setting the **text-decoration** property to none, for example:
```html
a.plain { text-decoration: none; }
```
Which would suppress underlining for a link such as:
```html
This is <a class="plain" href="what.html">not underlined</a>
```
Most people when they see underlined text on a Web page, will expect it to be part of a hypertext link. As a result, you are advised to leave underlining on for hypertext links. A similar argument applies to the link colors, most people will interpret underlined blue text as hypertext links. You are advised to leave link colors alone.

#### Color Blindness
When using color, remember that 5 to 10% of men have some form of color blindness. This can cause difficulties distinguishing between red and green, or between yellow and blue. In rare cases, there is an inability to perceive any colors at all. **You are recommended to avoid foreground/background color combinations that would make the text hard to read for people with color blindness.**
(优先使用文字排版进行页面结构区分，避免使用颜色进行前景/背景区分)

#### Named colors
The standard set of color names is: aqua, black, blue, fuchsia, gray, green, lime, maroon, navy, olive, purple, red, silver, teal, white, and yellow. These 16 colors are defined in HTML 3.2 and 4.01 and correspond to the basic VGA set on PCs.

#### Hexadecimal color values
Values like "#FF9999" represent colors as hexadecimal numbers for red, green and blue.

#### Browser safe colors
New computers support thousands or millions of colors, but many older color systems can only show up to 256 colors at a time.

Most browsers support the same so called "browser safe" palette. This uses 6 evenly spaced gradations in red, green and blue and their combinations. If you select image colors from this palette, you can avoid the speckling effect. This is particularly useful for background areas of images.


## What about browsers that don't support CSS?
Older browsers, that is to say before Netscape 4.0 and Internet Explorer 4.0, either don't support CSS at all or do so inconsistently. For these browsers you can still control the style by using HTML itself.


## Getting Further Information
For further information on CSS and tools that support it, you should look at the [W3C home page for CSS](https://www.w3.org/Style/CSS/). 


##### reference: 
[1] https://www.w3.org/MarkUp/Guide/Style.html
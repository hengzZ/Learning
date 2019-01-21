## Getting Started with Javascript
* *Javascript is a lightweight, interpreted programming language.*
* It is designed for creating network-centric applications.
* Javascript is very easy to implement because it is integrated with HTML.

This is a short guide to help you build dynamic web pages and web applications.


## Your First Javascript Program
\<script>... ***CODES*** ...\</script>
```html
<html>
   <body>
      <script language="javascript" type="text/javascript">
         <!--
            document.write("Hello World!")
         //-->
      </script>
   </body>
</html>
```
Attr:
- *language* or *type* -> **source code**
- *src* -> **include**


## Javascript - Placement in html file
* in *\<head>...\</head>* section.
* in *\<body>...\</body>* section.
* in *\<body>...\</body>* and *\<head>...\</head>* sections.
* in an external file and then *include in \<head>...\</head>* section.


## Javascript - Syntax
The *\<script>* tag alerts the browser program to start interpreting all the text between these tags as a script.
```html
<script language="javascript" type="text/javascript">
   javascript code
</script>
```
* *分号结尾* - simple statements in javascript are generally followed by a semicolon character, just as they are in c, c++, and java.
* *大小写敏感* - javascript is a case-sensitive language.
* *标准c注释风格* - javascript supports both c-style and c++-style comments.


## JavaScript - Variables
Three ***primitive*** data types:
* ***Numbers*** eg: 123, 120.50 etc.
* ***Strings*** eg: "This text string" etc.
* ***Boolean*** eg: *true* or *false*.

*Javascript also defines two trivial data types, **null** and **undefined**.*

*Javascript is untyped language and variables are declared with **var***
```html
<script type="text/javascript">
   <!--
      var name = "Ali";
      var money;
      money = 2000.50;
   //-->
</script>
```

Javascript variables have only two scopes.
* *Global Scope* - This means the variable can be defined anywhere in your javascript code.
* *Local Scope* - This variable will be visible only within the function where it is defined. Function parameters are always local to that function.
```html
<html>
   <body onload = checkscope();>
      <script type = "text/javascript">
         <!--
            var myVar = "global";  // declare a global variable
            function checkscope( ) {
               var myVar = "local";  // declare a local variable
               document.write(myVar);
            }
         //-->
      </script>
   </body>
</html>
```
*If you declare a local variable or function parameter with the same name as a global variable, you effectively hide the global variable.*


## JavaScript - Control Statement
* if-else
```html
<html>
   <body>
   
      <script type="text/javascript">
         <!--
            var book = "maths";
            if( book == "history" ){
               document.write("<b>History Book</b>");
            }
         
            else if( book == "maths" ){
               document.write("<b>Maths Book</b>");
            }
         
            else if( book == "economics" ){
               document.write("<b>Economics Book</b>");
            }
         
            else{
               document.write("<b>Unknown Book</b>");
            }
         //-->
      </script>
      
      <p>Set the variable to different value and then try...</p>
   </body>
</html>
```
* switch
```html
<html>
   <body>
   
      <script type="text/javascript">
         <!--
            var grade='A';
            document.write("Entering switch block<br />");
            switch (grade)
            {
               case 'A': document.write("Good job<br />");
               break;
            
               case 'B': document.write("Pretty good<br />");
               break;
            
               case 'C': document.write("Passed<br />");
               break;
            
               case 'D': document.write("Not so good<br />");
               break;
            
               case 'F': document.write("Failed<br />");
               break;
            
               default:  document.write("Unknown grade<br />")
            }
            document.write("Exiting switch block");
         //-->
      </script>
      
      <p>Set the variable to different value and then try...</p>
   </body>
</html>
```
* while/do-while
```html
<html>
   <body>
      
      <script type="text/javascript">
         <!--
            var count = 0;
            document.write("Starting Loop ");
         
            while (count < 10){
               document.write("Current Count : " + count + "<br />");
               count++;
            }
         
            document.write("Loop stopped!");
         //-->
      </script>
      
      <p>Set the variable to different value and then try...</p>
   </body>
</html>
```
* for
```html
<html>
   <body>
      
      <script type="text/javascript">
         <!--
            var count;
            document.write("Starting Loop" + "<br />");
         
            for(count = 0; count < 10; count++){
               document.write("Current Count : " + count );
               document.write("<br />");
            }
         
            document.write("Loop stopped!");
         //-->
      </script>
      
      <p>Set the variable to different value and then try...</p>
   </body>
</html>
```
* for...in
```html
<html>
   <body>
      
      <script type="text/javascript">
         <!--
            var aProperty;
            document.write("Navigator Object Properties<br /> ");
         
            for (aProperty in navigator) {
               document.write(aProperty);
               document.write("<br />");
            }
            document.write ("Exiting from the loop!");
         //-->
      </script>
      
      <p>Set the variable to different object and then try...</p>
   </body>
</html>
```


## JavaScript - Functions
The most common way to define a function in JavaScript is by using the **function** keyword.

Try the following example. It defines a function that takes two parameters and concatenates them before returning the resultant in the calling program.
```html
<html>
   <head>
      
      <script type="text/javascript">
         function concatenate(first, last)
         {
            var full;
            full = first + last;
            return full;
         }
         
         function secondFunction()
         {
            var result;
            result = concatenate('Zara', 'Ali');
            document.write (result );
         }
      </script>
      
   </head>
   
   <body>
      <p>Click the following button to call the function</p>
      
      <form>
         <input type="button" onclick="secondFunction()" value="Call Function">
      </form>
      
      <p>Use different parameters inside the function and then try...</p>
  
  </body>
</html>
```
There is a lot to learn about JavaScript functions, such as:
* JavaScript Nested Functions
* JavaScript Function( ) Constructor
* JavaScript Function Literals


## JavaScript - Events
**What is an Event ?** <br>
JavaScript's interaction with HTML is handled through events that occur when the user or the browser manipulates a page.

Here we will see a onclick Event example to understand a relation between Event and JavaScript −
```html
<html>
   <head>
      
      <script type="text/javascript">
         <!--
            function sayHello() {
               alert("Hello World")
            }
         //-->
      </script>
      
   </head>
   
   <body>
      <p>Click the following button and see result</p>
      
      <form>
         <input type="button" onclick="sayHello()" value="Say Hello" />
      </form>
      
   </body>
</html>
```
Please go through [a small tutorial](https://www.tutorialspoint.com/html/html_events_ref.htm) for a better understanding HTML Event Reference.


## JavaScript - Cookies
**What are Cookies ?** <br>
Cookies is the most efficient method of remembering and tracking preferences, purchases, commissions, and other information required for better visitor experience or site statistics.

Cookies are a plain text data record of 5 variable-length fields −
* **Expires** − The date the cookie will expire. If this is blank, the cookie will expire when the visitor quits the browser.
* **Domain** − The domain name of your site.
* **Path** − The path to the directory or web page that set the cookie. This may be blank if you want to retrieve the cookie from any directory or page.
* **Secure** − If this field contains the word "secure", then the cookie may only be retrieved with a secure server. If this field is blank, no such restriction exists.
* **Name=Value** − Cookies are set and retrieved in the form of key-value pairs

Cookies were originally designed for CGI programming, so CGI scripts on the server can read and write cookie values that are stored on the client. JavaScript can also manipulate cookies using the *cookie* property of the *Document* object. JavaScript can read, create, modify, and delete the cookies that apply to the current web page.
```html
<html>
   <head>
   
      <script type="text/javascript">
         <!--
            function WriteCookie()
            {
               var now = new Date();
               now.setMonth( now.getMonth() + 1 );
               cookievalue = escape(document.myform.customer.value) + ";"
               
               document.cookie="name=" + cookievalue;
               document.cookie = "expires=" + now.toUTCString() + ";"
               document.write ("Setting Cookies : " + "name=" + cookievalue );
            }
         //-->
      </script>
      
   </head>
   <body>
   
      <form name="myform" action="">
         Enter name: <input type="text" name="customer"/>
         <input type="button" value="Set Cookie" onclick="WriteCookie()"/>
      </form>
      
   </body>
</html>
```
Note − Cookie values may not include semicolons, commas, or whitespace. For this reason, you may want to use the JavaScript escape() function to encode the value before storing it in the cookie. If you do this, you will also have to use the corresponding unescape() function when you read the cookie value.


## JavaScript - Page Redirection
To redirect your site visitors to a new page, you just need to add a line in your head section as follows.
```html
<html>
   <head>
   
      <script type="text/javascript">
         <!--
            function Redirect() {
               window.location="http://www.tutorialspoint.com";
            }
            
            document.write("You will be redirected to main page in 10 sec.");
            setTimeout('Redirect()', 10000);
         //-->
      </script>
      
   </head>
   
   <body>
   </body>
</html>
```
Here setTimeout() is a built-in JavaScript function which can be used to execute another function after a given time interval.


## JavaScript - Dialog Boxes
JavaScript supports three important types of dialog boxes. These dialog boxes can be used to **raise an alert**, or to **get confirmation on any input** or to have a kind of **input from the users**.
```html
<html>
   <head>
   
      <script type="text/javascript">
         <!--
            function Warn() {
               alert ("This is a warning message!");
               document.write ("This is a warning message!");
            }
         //-->
      </script>
      
   </head>
   <body>
      <p>Click the following button to see the result: </p>
      
      <form>
         <input type="button" value="Click Me" onclick="Warn();" />
      </form>
      
   </body>
</html>
```
A confirmation dialog box is mostly used to take user's consent on any option. If the user clicks on the OK button, the window method **confirm()** will return true. If the user clicks on the Cancel button, then confirm() returns false. 
```html
<html>
   <head>
   
      <script type="text/javascript">
         <!--
            function getConfirmation(){
               var retVal = confirm("Do you want to continue ?");
               if( retVal == true ){
                  document.write ("User wants to continue!");
                  return true;
               }
               else{
                  document.write ("User does not want to continue!");
                  return false;
               }
            }
         //-->
      </script>
      
   </head>
   <body>
      <p>Click the following button to see the result: </p>
      
      <form>
         <input type="button" value="Click Me" onclick="getConfirmation();" />
      </form>
      
   </body>
</html>
```
The prompt dialog box is very useful when you want to pop-up a text box to get user input. This dialog box is displayed using a method called **prompt()** which takes two parameters: (i) a label which you want to display in the text box and (ii) a default string to display in the text box.
```html
<html>
   <head>
      
      <script type="text/javascript">
         <!--
            function getValue(){
               var retVal = prompt("Enter your name : ", "your name here");
               document.write("You have entered : " + retVal);
            }
         //-->
      </script>
      
   </head>
   
   <body>
      <p>Click the following button to see the result: </p>
      
      <form>
         <input type="button" value="Click Me" onclick="getValue();" />
      </form>
      
   </body>
</html>
```
This dialog box has two buttons: OK and Cancel. If the user clicks the OK button, the window method prompt() will return the entered value from the text box. If the user clicks the Cancel button, the window method prompt() returns null.


## JavaScript - Void Keyword
**Void** operator specifies an expression to be evaluated without returning a value.

The syntax of void can be either of the following two −
```html
<head>

   <script type="text/javascript">
      <!--
         void func()
         javascript:void func()
      
         or:
      
         void(func())
         javascript:void(func())
      //-->
   </script>
   
</head>
```
Here the expression "0" is evaluated, but it is not loaded back into the current document.
```html
<html>
   <head>
   
      <script type="text/javascript">
         <!--
         //-->
      </script>
      
   </head>
   <body>
   
      <p>Click the following, This won't react at all...</p>
      <a href="javascript:void(0)">Click me!</a>
      
   </body>
</html>
```


## JavaScript - Objects
JavaScript is an Object Oriented Programming (OOP) language - 
* Encapsulation − the capability to store related information, whether data or methods, together in an object.
* Aggregation − the capability to store one object inside another object.
* Inheritance − the capability of a class to rely upon another class (or number of classes) for some of its properties and methods.
* Polymorphism − the capability to write one function or method that works in a variety of different ways.

#### The Object() Constructor
* way 1: use **var** keyword
```html
<html>
   <head>
      <title>User-defined objects</title>
      
      <script type="text/javascript">
         var book = new Object();   // Create the object
         book.subject = "Perl"; // Assign properties to the object
         book.author  = "Mohtashim";
      </script>
      
   </head>
   
   <body>
   
      <script type="text/javascript">
         document.write("Book name is : " + book.subject + "<br>");
         document.write("Book author is : " + book.author + "<br>");
      </script>
   
   </body>
</html>
```
* way 2: use **this** keyword
```html
<html>
   <head>
   
   <title>User-defined objects</title>
   
      <script type="text/javascript">
         function book(title, author){
            this.title = title; 
            this.author  = author;
         }
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
      </script>
      
   </body>
</html>
```

#### Defining Methods for an Object
Use **this** keyword in the function
```html
<html>
   <head>
   <title>User-defined objects</title>
   
      <script type="text/javascript">
         // Define a function which will work as a method
         function addPrice(amount){
            this.price = amount; 
         }
         
         function book(title, author){
            this.title = title;
            this.author  = author;
            this.addPrice = addPrice; // Assign that method as property.
         }
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         myBook.addPrice(100);
         
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
         document.write("Book price is : " + myBook.price + "<br>");
      </script>
      
   </body>
</html>
```

#### The 'with' Keyword
The object specified as an argument to with becomes the default object for the duration of the block that follows. 
```html
<html>
   <head>
   <title>User-defined objects</title>
   
      <script type="text/javascript">
         // Define a function which will work as a method
         function addPrice(amount){
            with(this){
               price = amount; // this statement uses price instead of this.price
            }
         }
         
         function book(title, author){
            this.title = title;
            this.author  = author;
            this.price = 0;
            this.addPrice = addPrice; // Assign that method as property.
         }
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         myBook.addPrice(100);
         
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
         document.write("Book price is : " + myBook.price + "<br>");
      </script>
      
   </body>
</html>
```

#### JavaScript Native Objects
Here is the list of all important JavaScript Native Objects −
* JavaScript **Number** Object
* JavaScript **Boolean** Object
* JavaScript **String** Object
* JavaScript **Array** Object
* JavaScript **Date** Object
* JavaScript **Math** Object
* JavaScript **RegExp** Object


## JavaScript - Document Object Model or DOM
Every web page resides inside a browser window which can be considered as an object. A Document object represents the HTML document that is displayed in that window.
```html
<html>
   
   <head>
      <title> Document Title </title>
      
      <script type="text/javascript">
         <!--
            function myFunc()
            {
               var ret = document.getElementsByTagName("title");
               alert("Document Title : " + ret[0].text );
               
               var ret = document.getElementById("heading");
               alert(ret.innerHTML );
            }
         //-->
      </script>
      
   </head>
   <body>
      <h1 id="heading">This is main title</h1>
      <p>Click the following to see the result:</p>
      
      <form id="form1" name="FirstForm">
         <input type="button" value="Click Me" onclick="myFunc();" />
         <input type="button" value="Cancel">
      </form>
      
      <form d="form2" name="SecondForm">
         <input type="button" value="Don't ClickMe"/>
      </form>
      
   </body>
</html>
```
Note: In the above codes, **document** is the DOM object representing current HTML document.


## JavaScript - Errors & Exceptions Handling
The try...catch...finally Statement
```html
<html>
   
   <head>
      
      <script type="text/javascript">
         <!--
            function myFunc()
            {
               var a = 100;
               
               try {
                  alert("Value of variable a is : " + a );
               }
               
               catch ( e ) {
                  alert("Error: " + e.description );
               }
               
               finally {
                  alert("Finally block will always execute!" );
               }
            }
         //-->
      </script>
      
   </head>
   
   <body>
      <p>Click the following to see the result:</p>
      
      <form>
         <input type="button" value="Click Me" onclick="myFunc();" />
      </form>
      
   </body>
</html>
```

# Samples
## JavaScript - Form Validation
```html
<html>
   
   <head>
      <title>Form Validation</title>
      
      <script type="text/javascript">
         <!--
            // Form validation code will come here.
         //-->
      </script>
      
   </head>
   
   <body>
      <form action="/cgi-bin/test.cgi" name="myForm" onsubmit="return(validate());">
         <table cellspacing="2" cellpadding="2" border="1">
            
            <tr>
               <td align="right">Name</td>
               <td><input type="text" name="Name" /></td>
            </tr>
            
            <tr>
               <td align="right">EMail</td>
               <td><input type="text" name="EMail" /></td>
            </tr>
            
            <tr>
               <td align="right">Zip Code</td>
               <td><input type="text" name="Zip" /></td>
            </tr>
            
            <tr>
               <td align="right">Country</td>
               <td>
                  <select name="Country">
                     <option value="-1" selected>[choose yours]</option>
                     <option value="1">USA</option>
                     <option value="2">UK</option>
                     <option value="3">INDIA</option>
                  </select>
               </td>
            </tr>
            
            <tr>
               <td align="right"></td>
               <td><input type="submit" value="Submit" /></td>
            </tr>
            
         </table>
      </form>
      
   </body>
</html>
```
In the above form, we are calling validate() to validate data when onsubmit event is occurring. The below example shows how to do a basic form validation:
```html
<script type="text/javascript">
   <!--
      // Form validation code will come here.
      function validate()
      {
      
         if( document.myForm.Name.value == "" )
         {
            alert( "Please provide your name!" );
            document.myForm.Name.focus() ;
            return false;
         }
         
         if( document.myForm.EMail.value == "" )
         {
            alert( "Please provide your Email!" );
            document.myForm.EMail.focus() ;
            return false;
         }
         
         if( document.myForm.Zip.value == "" ||
         isNaN( document.myForm.Zip.value ) ||
         document.myForm.Zip.value.length != 5 )
         {
            alert( "Please provide a zip in the format #####." );
            document.myForm.Zip.focus() ;
            return false;
         }
         
         if( document.myForm.Country.value == "-1" )
         {
            alert( "Please provide your country!" );
            return false;
         }
         return( true );
      }
   //-->
</script>
```
The following example shows how to validate an entered email address:
```html
<script type="text/javascript">
   <!--
      function validateEmail()
      {
         var emailID = document.myForm.EMail.value;
         atpos = emailID.indexOf("@");
         dotpos = emailID.lastIndexOf(".");
         
         if (atpos < 1 || ( dotpos - atpos < 2 )) 
         {
            alert("Please enter correct email ID")
            document.myForm.EMail.focus() ;
            return false;
         }
         return( true );
      }
   //-->
</script>
```


##### reference
[1] https://www.tutorialspoint.com/javascript/index.htm

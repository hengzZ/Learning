## First Java Program
File: [MyFirstJavaProgram.java]  
```java
public class MyFirstJavaProgram {
    
    public static void main (String[] args) {
        System.out.println("Hello World");
    }
}
```
Compiling:  <br>
`javac MyFirstJavaProgram.java`  <br>
Running:  <br>
`java MyFirstJavaProgram`  <br>
Output:  <br>
`Hello World`  


## Basic Syntax
About Java programs, it is very important to keep in mind the following points.

* Case Sensitivity − Java is case sensitive, which means identifier Hello and hello would have different meaning in Java.

* **Class Names** − For all class names the first letter should be in Upper Case. If several words are used to form a name of the class, each inner word's first letter should be in Upper Case.

    Example: class MyFirstJavaClass

* **Method Names** − All method names should start with a Lower Case letter. If several words are used to form the name of the method, then each inner word's first letter should be in Upper Case.

    Example: public void myMethodName()

* **Program File Name** − Name of the program file should exactly match the class name.

    When saving the file, you should save it using the class name (Remember Java is case sensitive) and append '.java' to the end of the name (if the file name and the class name do not match, your program will not compile).

    Example: Assume 'MyFirstJavaProgram' is the class name. Then the file should be saved as 'MyFirstJavaProgram.java'

* **public static void main(String args[])** − Java program processing starts from the main() method which is a mandatory part of every Java program.


## Java Modifiers
Like other languages, it is possible to modify classes, methods, etc., by using modifiers. There are two categories of modifiers −

* Access Modifiers − default, public , protected, private
* Non-access Modifiers − static, final, abstract, synchronized, volatile


## Java Variables
Following are the types of variables in Java −

* Local Variables
* Class Variables (Static Variables)
* Instance Variables (Non-static Variables)


## Creating an Object
There are three steps when creating an object from a class -

* *Delcaration* - A variable declaration with a variable name with an object type.
* *Instantiation* - The 'new' keyword is used to create the object.
* *Initialization* - The 'new' keyword is followed by a call to a constructor. This call initializes the new object.

    Example
    ```java
    public class Puppy {
        public Puppy (String name) {
            System.out.println ("Passed Name is :" + name);
        }
        
        public static void main (String[] args) {
            Puppy myPuppy = new Puppy ("tommy");
        }
    }
    ```
    
    
## Source File Declaration Rules
These rules are essential when *declaring classes*, *import statements* and *package statements* in a source file -

* There can be only **one public class** per source file.

* A source file can have **multiple non-public classes**.

* The public class name should be the name of the source file as well which should be appended by **.java** at the end. 

* If the class is defined inside a package, then the package statement should be the first statement in the source file.

* If import statements are present, then they must be written between the package statement and the class declaration. If there are no package statements, then the import statement should be the first line in the source file.

* Import and package statements will imply to all the classes present in the source file. It is not possible to declare different import and/or package statements to different classes in the source file.


## Java Package and Import Statements
* **Java Package** - In simple words, it is a way of **categorizing** the classes and interfaces. When developing applications in Java, hundreds of classes and interfaces will be written, therefore categorizing these classes is a must as well as makes life much easier.
* **Import Statements** - It is a way of giving the proper **location** for the compiler to find that particular class.


## Constructors
1. It has the same **name** as its class.
1. It has no explicit **return** type.


## Basic Datatypes
There are two data types available in Java -
* **Primitive** Data Types
    
    Integral Values(整数)
    * byte - (8bit signed) used to save space in large arrays, mainly in place of integers.
    * short - (16bit signed) also be used to save memory as byte data type.
    * int - (32bit signed) is generally used as the default data type for **integral values**.
    * long - (64bit signed) used when a wider range than int is needed.

    Decimal Values(十进制数)
    * float - (32bit floating) mainly used to save memory in large arrays of floating point numbers.
    * double - (64bit floating) is generally used as the default data type for **decimal values**.
    
    Precise Values(such as currency)
    * **float/double data type should never be used for precise values !!!**.
    
    Boolean
    * boolean -  used for simple flags that track true/false conditions.
    
    Char
    * char - used to store **16bit** Unicode character.
    
* **Reference/Object** Data Types


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


## Java - Basic Syntax
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


## Java - Modifiers
Like other languages, it is possible to modify classes, methods, etc., by using modifiers. There are two categories of modifiers −

* Access Modifiers − default, public , protected, private
* Non-access Modifiers − static, final, abstract, synchronized, volatile


## Java - Variables
Following are the types of variables in Java −

* Local Variables
* Class Variables (Static Variables)
* Instance Variables (Non-static Variables)


## Java - Creating an Object
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
    
    
## Java - Source File Declaration Rules
These rules are essential when *declaring classes*, *import statements* and *package statements* in a source file -

* There can be only **one public class** per source file.

* A source file can have **multiple non-public classes**.

* The public class name should be the name of the source file as well which should be appended by **.java** at the end. 

* If the class is defined inside a package, then the package statement should be the first statement in the source file.

* If import statements are present, then they must be written between the package statement and the class declaration. If there are no package statements, then the import statement should be the first line in the source file.

* Import and package statements will imply to all the classes present in the source file. It is not possible to declare different import and/or package statements to different classes in the source file.


## Java - Package and Import Statements
* **Java Package** - In simple words, it is a way of **categorizing** the classes and interfaces. When developing applications in Java, hundreds of classes and interfaces will be written, therefore categorizing these classes is a must as well as makes life much easier.
* **Import Statements** - It is a way of giving the proper **location** for the compiler to find that particular class.


## Constructors
1. It has the same **name** as its class.
1. It has no explicit **return** type.


## Java - Basic Datatypes
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


## Java - Regex
The java.util.regex package primarily consists of the following three classes −
* **Pattern Class** − A Pattern object is a compiled representation of a regular expression. The Pattern class provides no public constructors. To create a pattern, you must first invoke one of its public static **compile()** methods, which will then return a Pattern object. These methods accept a regular expression as the first argument.

* **Matcher Class** − A Matcher object is the engine that interprets the pattern and performs match operations against an input string. Like the Pattern class, Matcher defines no public constructors. You obtain a Matcher object by invoking the **matcher()** method on a Pattern object.

* **PatternSyntaxException** − A PatternSyntaxException object is an unchecked exception that indicates a syntax error in a regular expression pattern.

    Example:
    ```java
    import java.util.regex.Matcher;
    import java.util.regex.Pattern;

    public class RegexMatches {
    
        public static void main( String args[] ) {
            // String to be scanned to find the pattern.
            String line = "This order was placed for QT3000! OK?";
            String pattern = "(.*)(\\d+)(.*)";
    
            // Create a Pattern object
            Pattern r = Pattern.compile(pattern);
    
            // Now create matcher object.
            Matcher m = r.matcher(line);
            if (m.find( )) {
                System.out.println("Found value: " + m.group(0) );
                System.out.println("Found value: " + m.group(1) );
                System.out.println("Found value: " + m.group(2) );
            }else {
                System.out.println("NO MATCH");
            }
        }
    }
    ```


## Java - Exceptions
* When an **Exception** occurs the normal flow of the program is disrupted and the program/Application terminates abnormally, which is not recommended, therefore, these exceptions are to be handled.

* Exceptions are caused by **user error**, others by **programmer error**, and others by **physical resources** that have failed in some manner.

* Java has three categories of Exceptions-
    * **Checked exceptions** − A checked exception is an exception that is checked (notified) by the compiler at compilation-time, these are also called as compile time exceptions. These exceptions cannot simply be ignored, the programmer should take care of (handle) these exceptions.
    * **Unchecked exceptions** − An unchecked exception is an exception that occurs at the time of execution. These are also called as **Runtime Exceptions**. These include programming bugs, such as logic errors or improper use of an API. Runtime exceptions are ignored at the time of compilation.
    * **Errors** − These are not exceptions at all, but problems that arise beyond the control of the user or the programmer. **Errors are typically ignored in your code because you can rarely do anything about an error**. For example, if a stack overflow occurs, an error will arise. They are also ignored at the time of compilation.
    
### The try-with-resources
Generally, when we use any resources like streams, connections, etc. we have to close them explicitly using finally block. It also referred as **automatic resource management**.

```java
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class ReadData_Demo {

   public static void main(String args[]) {
      FileReader fr = null;		
      try {
         File file = new File("file.txt");
         fr = new FileReader(file); char [] a = new char[50];
         fr.read(a);   // reads the content to the array
         for(char c : a)
         System.out.print(c);   // prints the characters one by one
      } catch (IOException e) {
         e.printStackTrace();
      }finally {
         try {
            fr.close();
         } catch (IOException ex) {		
            ex.printStackTrace();
         }
      }
   }
}
```


## Java - Inner classes
* **Non-static nested classes**
    
    * Inner Class
    ```java
    class Outer_Demo {
        // private variable of the outer class
        private int num = 175;  
   
        // inner class
        public class Inner_Demo {
            public int getNum() {
                System.out.println("This is the getnum method of the inner class");
                return num;
            }
        }
    }

    public class My_class2 {

        public static void main(String args[]) {
            // Instantiating the outer class
            Outer_Demo outer = new Outer_Demo();
      
            // Instantiating the inner class
            Outer_Demo.Inner_Demo inner = outer.new Inner_Demo();
            System.out.println(inner.getNum());
        }
    }
    ```
    
    * Method-local Inner Class
    ```java
    public class Outerclass {
        // instance method of the outer class 
        void my_Method() {
            int num = 23;

            // method-local inner class
            class MethodInner_Demo {
                public void print() {
                    System.out.println("This is method inner class "+num);	   
                }   
            } // end of inner class
	   
            // Accessing the inner class
            MethodInner_Demo inner = new MethodInner_Demo();
            inner.print();
        }
   
        public static void main(String args[]) {
            Outerclass outer = new Outerclass();
            outer.my_Method();	   	   
        }
    }
    ```
    
    * Anonymous Inner Class
        1. In case of anonymous inner classes, we declare and instantiate them at the same time.
        1. Generally, if a method accepts an object of an interface, an abstract class, or a concrete class, then we can implement the interface, extend the abstract class, and pass the object to the method.
        ```java
        abstract class AnonymousInner {
            public abstract void mymethod();
        }
    
        public class Outer_class {
    
            public static void main(String args[]) {
                AnonymousInner inner = new AnonymousInner() {
                    public void mymethod() {
                        System.out.println("This is an example of anonymous inner class");
                    }
                };
                inner.mymethod();	
            }
        }
        ```
        ```java
        // interface
        interface Message {
            String greet();
        }

        public class My_class {
            // method which accepts the object of interface Message
            public void displayMessage(Message m) {
                System.out.println(m.greet() +
                    ", This is an example of anonymous inner class as an argument");  
            }

            public static void main(String args[]) {
                // Instantiating the class
                My_class obj = new My_class();

                // Passing an anonymous inner class as an argument
                obj.displayMessage(new Message() {
                    public String greet() {
                        return "Hello";
                    }
                });
            }
        }
        ```

* **Static nested classes**
```java
public class Outer {
   static class Nested_Demo {
      public void my_method() {
         System.out.println("This is my nested class");
      }
   }
   
   public static void main(String args[]) {
      Outer.Nested_Demo nested = new Outer.Nested_Demo();	 
      nested.my_method();
   }
}
```


## Java - Inheritance （继承）
```java
class Superclass {
   int age;

   Superclass(int age) {
      this.age = age; 		 
   }

   public void getAge() {
      System.out.println("The value of the variable named age in super class is: " +age);
   }
}

public class Subclass extends Superclass {
   Subclass(int age) {
      super(age);
   }

   public static void main(String argd[]) {
      Subclass s = new Subclass(24);
      s.getAge();
   }
}
```

* Inheritance can be defined as the process where one class acquires the properties (methods and fields) of another.

* A very important fact to remember is that Java does not support multiple inheritance.

* **Relationship of Two Objects**
    * IS-A is a way of saying: This object is a type of that object. Let us see how the extends keyword is used to achieve inheritance.
    ```java
    public class Animal {
    }

    public class Mammal extends Animal {
    }

    public class Reptile extends Animal {
    }
    
    public class Dog extends Mammal {
    }
    ```
    * HAS-A relationship. This determines whether a certain class HAS-A certain thing. This relationship helps to reduce duplication of code as well as bugs.
    ```java
    public class Vehicle{}
    public class Speed{}
    
    public class Van extends Vehicle {
       private Speed sp;
    }
    ```


## Java - Overriding （覆盖）

```java
class Animal {
   public void move() {
      System.out.println("Animals can move");
   }
}

class Dog extends Animal {
   public void move() {
      System.out.println("Dogs can walk and run");
   }
}

public class TestDog {

   public static void main(String args[]) {
      Animal a = new Animal();   // Animal reference and object
      Animal b = new Dog();   // Animal reference but Dog object

      a.move();   // runs the method in Animal class
      b.move();   // runs the method in Dog class
   }
}
```

**super** keyword: When invoking a superclass version of an overridden method the super keyword is used.
```java
class Animal {
   public void move() {
      System.out.println("Animals can move");
   }
}

class Dog extends Animal {
   public void move() {
      super.move();   // invokes the super class method
      System.out.println("Dogs can walk and run");
   }
}

public class TestDog {

   public static void main(String args[]) {
      Animal b = new Dog();   // Animal reference but Dog object
      b.move();   // runs the method in Dog class
   }
}
```


## Java - Polymorphism （多态）
The most common use of polymorphism in OOP occurs when a parent class reference is used to refer to a child class object.
```java
public interface Vegetarian{}
public class Animal{}
public class Deer extends Animal implements Vegetarian{}

Example:
Deer d = new Deer();
Animal a = d;
Vegetarian v = d;
Object o = d;
```


## Java - Abstraction (纯虚)
```java
/* File name : Employee.java */
public abstract class Employee {
   private String name;
   private String address;
   private int number;

   public Employee(String name, String address, int number) {
      System.out.println("Constructing an Employee");
      this.name = name;
      this.address = address;
      this.number = number;
   }
   
   public double computePay() {
     System.out.println("Inside Employee computePay");
     return 0.0;
   }
   
   public void mailCheck() {
      System.out.println("Mailing a check to " + this.name + " " + this.address);
   }

   public String toString() {
      return name + " " + address + " " + number;
   }

   public String getName() {
      return name;
   }
 
   public String getAddress() {
      return address;
   }
   
   public void setAddress(String newAddress) {
      address = newAddress;
   }
 
   public int getNumber() {
      return number;
   }
}
```


## Java - Encapsulation (封装)
```java
/* File name : EncapTest.java */
public class EncapTest {
   private String name;
   private String idNum;
   private int age;

   public int getAge() {
      return age;
   }

   public String getName() {
      return name;
   }

   public String getIdNum() {
      return idNum;
   }

   public void setAge( int newAge) {
      age = newAge;
   }

   public void setName(String newName) {
      name = newName;
   }

   public void setIdNum( String newId) {
      idNum = newId;
   }
}
```
The public setXXX() and getXXX() methods are the access points of the instance variables of the EncapTest class. Normally, these methods are referred as getters and setters. Therefore, any class that wants to access the variables should access them through these getters and setters.


## Java - Interfaces (API)
An interface is a reference type in Java. It is similar to class. It is a collection of abstract methods.<br>
The **interface** keyword is used to declare an interface. Here is a simple example to declare an interface −
```java
/* File name : NameOfInterface.java */
import java.lang.*;
// Any number of import statements

public interface NameOfInterface {
   // Any number of final, static fields
   // Any number of abstract method declarations\
}
```
Note: 
* An interface is implicitly abstract. You do not need to use the abstract keyword while declaring an interface.
* Each method in an interface is also implicitly abstract, so the abstract keyword is not needed.
* Methods in an interface are implicitly public.
```java
/* File name : Animal.java */
interface Animal {
   public void eat();
   public void travel();
}
```


## Java - Packages （对应 C++ namespace 特性）
Packages are used in Java in order to prevent naming conflicts, to control access, to make searching/locating and usage of classes, interfaces, enumerations and annotations easier, etc.
```java
/* File name : Animal.java */
package animals;

interface Animal {
   public void eat();
   public void travel();
}

/* File name : MammalInt.java */
package animals;

public class MammalInt implements Animal {

   public void eat() {
      System.out.println("Mammal eats");
   }

   public void travel() {
      System.out.println("Mammal travels");
   } 

   public int noOfLegs() {
      return 0;
   }

   public static void main(String args[]) {
      MammalInt m = new MammalInt();
      m.eat();
      m.travel();
   }
} 
```
compile:
```bash
$ javac -d . Animal.java 
$ javac -d . MammalInt.java
```

## Java - Data Structures
The data structures provided by the Java utility package are very powerful and perform a wide range of functions. These data structures consist of the following interface and classes − Enumeration, BitSet, Vector, Stack, Dictionary, Hashtable, Properties.

## Java - Collections Framework
Prior to Java 2, Java provided ad hoc classes such as Dictionary, Vector, Stack, and Properties to store and manipulate groups of objects. Although these classes were quite useful, they lacked a central, unifying theme.

## Java - Generics (泛型)
Java Generic methods and generic classes enable programmers to specify, with a single method declaration, a set of related methods, or with a single class declaration, a set of related types, respectively.

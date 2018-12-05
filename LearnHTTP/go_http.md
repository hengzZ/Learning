# GO HTTP 小例子(信息交互)
**Host Code:** /* filename : go_http.go */
```go
package main

import   "io"
import   "log"
import   "net/http"


func sayHello(w http.ResponseWriter, r *http.Request) {
    io.WriteString(w, "Hello world,this is version one.")
}


func main() {
    //set route
    http.HandleFunc("/", sayHello)
    err := http.ListenAndServe(":8080", nil)  //default while the second parameter is nil
    if err != nil {
        log.Fatal(err)
    }
}
```
**Run**
```bash
go run go_http.go  # Start the server
```
```html
http://localhost:8080/  # User browser to request http://localhost:8080/
```

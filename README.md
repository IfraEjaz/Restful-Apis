RESTFUL Application 
-------------------
### Introduction
A REST API (also known as RESTful API) is an application programming interface (API or web API) that conforms to the constraints of REST architectural style and allows for interaction with RESTful web services. REST stands for representational state transfer and was created by computer scientist Roy Fielding. REST is a set of architectural constraints, not a protocol or a standard. API developers can implement REST in a variety of ways.

When a client request is made via a RESTful API, it transfers a representation of the state of the resource to the requester or endpoint. This information, or representation, is delivered in one of several formats via HTTP: JSON (Javascript Object Notation), HTML, XLT, Python, PHP, or plain text. JSON is the most generally popular file format to use because, despite its name, it’s language-agnostic, as well as readable by both humans and machines. 

Something else to keep in mind: Headers and parameters are also important in the HTTP methods of a RESTful API HTTP request, as they contain important identifier information as to the request's metadata, authorization, uniform resource identifier (URI), caching, cookies, and more. There are request headers and response headers, each with their own HTTP connection information and status codes.

In order for an API to be considered RESTful, it has to conform to these criteria:
* A client-server architecture made up of clients, servers, and resources, with requests managed through HTTP.
* Stateless client-server communication, meaning no client information is stored between get requests and each request is separate and unconnected.
* Cacheable data that streamlines client-server interactions.
* A uniform interface between components so that information is transferred in a standard form. This requires that:
    - resources requested are identifiable and separate from the representations sent to the client.
    - resources can be manipulated by the client via the representation they receive because the representation contains enough information to do so.
    - self-descriptive messages returned to the client have enough information to describe how the client should process it.
    - hypertext/hypermedia is available, meaning that after accessing a resource the client should be able to use hyperlinks to find all other currently available actions they can take.
* A layered system that organizes each type of server (those responsible for security, load-balancing, etc.) involved the retrieval of requested information into hierarchies, invisible to the client.
* Code-on-demand (optional): the ability to send executable code from the server to the client when requested, extending client functionality. 

Though the REST API has these criteria to conform to, it is still considered easier to use than a prescribed protocol like SOAP (Simple Object Access Protocol), which has specific requirements like XML messaging, and built-in security and transaction compliance that make it slower and heavier. 

In contrast, REST is a set of guidelines that can be implemented as needed, making REST APIs faster and more lightweight, with increased scalablity—perfect for Internet of Things (IoT) and mobile app development. 

In RESTFUL api, there are four frequently used function with the defination are enlisted below
* GET: The HTTP GET method is used to read (or retrieve) a representation of a resource in the supported formats (JSON, html etc). In an error case, it most often returns a 404 (NOT FOUND) or 400 (BAD REQUEST). 
* POST: The POST verb is most often utilized to create new resources. On successful creation, return HTTP status 201, returning a Location header with a link to the newly-created resource with the 201 HTTP status. 
* PUT: It is used for updating the capabilities. Sometimes, PUT is to a URI that contains the value of a non-existent resource ID. On successful update, return 200 (or 204 if not returning any content in the body) from a PUT.
* DELETE: It is used to delete a resource identified by a URI. On successful deletion, return HTTP status 200 (OK) along with a response body.

This repository contains the examples and implementation of RESTFUL Application Programming Interface using Python and C plus plus programming appraoch.

### Web Server Technologies
| WSGI    | ASGI |
| -------- | ------- |
| Web Server Gateway Interface  | Asynchronous Gateway Interface    |
| Handles one request at a time | Allows parallel processing of multiple requests at a time     |
| Blocks the server until the processing of one request is completed.    | Does not block the server    |
| Blocks the server until the processing of one request is completed.    | Does not block the server    |
| Traditional, so runs on majority web servers and hosting environments    | Newer protocol, so may require specific web servers and hosting environments    |
| Used for low to moderate traffic.   | Used for high traffic applications.    |
| waitress, gunicorn   | waitress, uvicorn |

Web Servers
-----------
We have use three webserver to deploy RESTful application. Two are based in python while the one is based on C++. We will provide a very breif introduction.

#### Python FAST API - ASGI Based 
FastAPI is a modern web framework first released in 2018 for building RESTful APIs in Python. It is used for building APIs with Python 3.8+ based on standard Python-type hints. FastAPI is based on Pydantic and uses type hints to validate, serialize and deserialize data

#### Python FLAST API - WSGI Based
Flask is a micro web framework written in Python. It is classified as a microframework because it does not require particular tools or libraries. It has no database abstraction layer, form validation, or any other components where pre-existing third-party libraries provide common functions.

#### C PLUS PLUS RESTBED - WSGI and ASGI Supported
Restbed is a comprehensive and consistent programming model for building applications that require seamless and secure communication over HTTP, with the ability to model a range of business processes, designed to target mobile, tablet, desktop and embedded production environments. Developed by corvusoft ltd UK.

Build Instructions
------------------
All Application Build Instruction are added in the sub-folders of respository. To see the further documentation please goto sub-directories.
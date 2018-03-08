# PerformanceMonitor

| Module | Status |
| ------ | ------ |
| Managed code HttpModule | [![Build status](https://ci.appveyor.com/api/projects/status/ej31e66aki3uryj6/branch/master?svg=true)](https://ci.appveyor.com/project/ktietjen/performancemonitor/branch/master) |
| Native code HttpModule | [![Build status](https://ci.appveyor.com/api/projects/status/ej31e66aki3uryj6/branch/master?svg=true)](https://ci.appveyor.com/project/ktietjen/performancemonitor/branch/master) |

The PerformanceMonitor project consists of two seperate IIS HttpModules with generally the same functionality.  One is written in C# and is therefore a managed code HttpModule and the other is written in C++ and is a native code HttpModule. Here is the list of current features of both:

1. Measure the total time spent processing a http request.
2. Measure the time spent by only the IHttpHandler for the request.
3. Measure the size of the response body in bytes and calculate the minimum, average, and maximum responses seen so far.
4. Inject measurements and calculation results into response.

There are some differences between the two solutions which is why I created both.  The fact that the C# HttpModule is managed code and executes in a normal .NET CLR means that it deals with Application Domains ([AppDomains](https://docs.microsoft.com/en-us/dotnet/framework/app-domains/application-domains)) which is an application isolation container to improve security and reliability.

The first major difference is that IIS/ASP.NET by default creates seperate AppDomains for web sites and web applications which means that a different copy of a managed HttpModule will normal be instantiated in each different AppDomain.  This means that the performance monitoring or benchmarking occurrs seperately for each AppDomain respectively.  This means that if you are attempting to use a managed code HttpModule to benchmark a website with web applications, the benchmarking would happen completely seperately for the site and each application.  Additionally IIS/ASP.NET creates new AppDomains when a configuration or code change happens and you can create seperate AppDomains for your own reasons.  The native code HttpModule does not have the same isolation and so the same HttpModule is applied across the website and applications and so the performance benchmarking is collectively across the entire system.

The second major difference is how the IIS/ASP.NET pipeline works a little different for managed code versus unmanaged or native code.  In general a managed code HttpModule can register for such events as BeginRequest (an event signaling the beginning of processing an http request) and EndRequest (an event signaling the end of processing an http request).  In contrast a native code HttpModule can register for the BeginRequest event and a new event right after the completion of the BeginRequest called the PostBeginRequest evet and the same goes for the EndRequest and other events which in benchmarking can cause slight differences in timing.

The third major difference is more the fact that the native code HttpModule has to deal with more fundamental Windows primatives and so results in more complexity for and in the native code.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* Microsoft Windows
* IIS 7+
* .NET v2.0+

### Installing

A step by step series of examples that tell you have to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Unit tests can be executed by Visual Studio or by msbuild.
* In Visual Studio, you bring up the Test Explorer and click to execute the unit tests.
* In msbuild when you build the solution the tests will automatically executed.

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Visual Studio 2017](https://www.visualstudio.com/downloads/) - Microsoft's Visual Studio


## Author

* **Ken Tietjen** - *Initial work* - [ktietjen](https://github.com/ktietjen)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details



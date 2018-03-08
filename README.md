# PerformanceMonitor

| Module | Status |
| ------ | ------ |
| Managed code HttpModule | [![Build status](https://ci.appveyor.com/api/projects/status/ej31e66aki3uryj6/branch/master?svg=true)](https://ci.appveyor.com/project/ktietjen/performancemonitor/branch/master) |
| Native code HttpModule | [![Build status](https://ci.appveyor.com/api/projects/status/80ujpm8pdkh9dsx6/branch/master?svg=true)](https://ci.appveyor.com/project/ktietjen/performancemonitor-v6dts/branch/master) |

The PerformanceMonitor project consists of two seperate IIS HttpModules with generally the same functionality.  One is written in C# and is therefore a managed code HttpModule and the other is written in C++ and is a native code HttpModule. Here is the list of current features of both:

1. Measure the total time spent processing a http request.
2. Measure the time spent by only the IHttpHandler for the request.
3. Measure the size of the response body in bytes and calculate the minimum, average, and maximum responses seen so far.
4. Inject measurements and calculation results into response.

There are some differences between the two solutions which is why I created both.  The fact that the C# HttpModule is managed code and executes in a normal .NET CLR means that it deals with Application Domains ([AppDomains](https://docs.microsoft.com/en-us/dotnet/framework/app-domains/application-domains)) which is an application isolation container to improve security and reliability.

The first major difference is that IIS/ASP.NET by default creates seperate AppDomains for web sites and web applications causing a different copy of the managed HttpModule to be instantiated in each different AppDomain.  This means that the performance monitoring or benchmarking occurrs seperately for each AppDomain respectively.  So, if you are attempting to use a managed code HttpModule to benchmark a website with web applications, the benchmarking would be completely seperate for the site and each application.  Additionally IIS/ASP.NET creates new AppDomains when a configuration or code change happens and you can create seperate AppDomains for your own reasons.  To overcome the AppDomain seperation you would have to marshal across the AppDomain repeatedly--a performance expensive operation that would negatively impact the benchmark results inspite of having nothing to do with the application under test which is very undesirable.  I chose not to implement the work-around. Alternatively, the native code HttpModule does not have the same isolation and so the same HttpModule is applied across the website and applications and so the performance benchmarking is collectively across the entire system without having to perform performance impacting operations.

The second major difference is how the IIS/ASP.NET pipeline works a little different for managed code versus unmanaged or native code.  In general a managed code HttpModule can register for such events as BeginRequest (an event signaling the beginning of processing an http request) and EndRequest (an event signaling the end of processing an http request).  In contrast a native code HttpModule can register for the BeginRequest event and a new event right after the completion of the BeginRequest called the PostBeginRequest event and the same goes for the EndRequest and many other events which enables the native HttpModule to reflect the underlying system more faithfully and be more accurate, flexible, expandable, and powerful.

All of this power and flexibility comes with a cost which is the third major difference--that the native code HttpModule native code is more complex and harder to maintain as it deals more directly with the complex underlying systems of Windows and IIS/ASP.NET.  The higher engineering effort to create and maintain a native code HttpModule is balanced with the resulting solution easier to use than the managed code solution.  A large part of the difficulty in using the managed code HttpModule has to do with the user experience being directly negatively impacted by a security feature of AppDomains and not being able to see the benchmarking as cohesive and comprehensive.

## Repository structure

The repository is organized like this:
PerformanceMonitor
      |
      |---- Managed (contains the Managed solution and files)
      |
      |---- Native (contains the native solution and files)

### Prerequisites

* Microsoft Windows
* IIS 7+
* .NET v2.0+

### Installing

Copy the repository locally either by downloading or by git.

#### Managed
Visual Studio:
1. Open managed solution in "PerformanceMonitor\Managed\PerformanceMonitor.sln"
2. Select the solution configuration desired Debug|Release
3. Copy built dll from target directory "PerformanceMonitor\Managed\PerformanceMonitor\bin\<target>"
  
#### Native
Visual Studio:
1. Open native solution in "PerformanceMonitor\Native\PerformanceMonitor.sln"
2. Select the solution configuration desired Debug|Release and platform (x64)
3. Copy built dll from target directory "PerformanceMonitor\Native\PerformanceMonitor\bin\<target>"

## Running the tests

Unit tests can be executed by Visual Studio or by msbuild.
* In Visual Studio, bring up the Test Explorer and click execute unit tests.

## Deployment

### Native
Copy native dll to local system deployment directory

Execute the 'install-native.cmd' script passing it the location of the native dll

Example: install-native.cmd "c:\inetpub\bin\PerformanceMonitor.dll"

### Managed
Copy managed dll to local system deployment directory. Note: this must be the root of the "Default Web Site" in a folder called bin.

Execute the 'install-managed.cmd' script.  The script will add the HttpModule to everything in the "Default Web Site" and any child applications.

Example: install-managed.cmd

## Built With

* [Visual Studio 2017](https://www.visualstudio.com/downloads/) - Microsoft's Visual Studio


## Author

* **Ken Tietjen** - *Initial work* - [ktietjen](https://github.com/ktietjen)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details



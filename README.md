BaaS QML Plug-in
==========================

This plug-in delivers an interface for Qt Quick developers for accessing the
APIs of Mobile Backend as a Service providers that allow creating mobile applications powered
by cloud services.


The main features of the plug-in are:

 - users related function (login/logout/register/delete user)
 - a QML model for showing content

 

The plug-in has a simple example.


The plug-in can be used as a pre-compiled shared library or you can copy the
source code into your project and compile it with your application. Although
the plug-in has been designed to be used with QML UI applications, it can also be
used as-is with Qt C++.



1. Prerequisites
-------------------------------------------------------------------------------

 - Qt basics
 - Qt Quick basics
 - Parse server installed - see [project details](https://github.com/ParsePlatform/parse-server)


2. Project structure
-------------------------------------------------------------------------------

 |                  The root folder contains the project file, the license
 |                  information, and this file (release notes).
 |
 |- doc             Contains documentation on the project in general (not yet actually).
 |
 |- examples        Contains a set of example applications that utilise the
 |                  plug-in.(only one sample for now)
 |
 |- plugin          Root folder of the plug-in implementation.
 |  |
 |  |- src          Plug-in source codes.
 |


3. Compatibility
-------------------------------------------------------------------------------



Developed with Qt 5.5.1.

3.1 Required capabilities
-------------------------
- Parse server up and running : see [project details](https://github.com/ParsePlatform/parse-server)

3.2 Known issues
----------------

 - The project is currently at a WIP stage, only Parse server is supported as a possible
backend
 - qmldir file is to be deployed manually to the plugin folder
 - for now, only a limited set of feature is supported (authentification mostly)



4. Building and using the plug-in
-------------------------------------------------------------------------------

4.1 Preparations
----------------

Check that you have the latest Qt SDK installed in the development environment
and the latest Qt version on the device.

4.2 Using the Qt SDK
--------------------

You can install and run the application on the device by using the Qt SDK.
Open the project in the SDK, set up the correct target (depending on the device
platform), and click the Compile button. 

4.3 Using the plug-in in your application
-----------------------------------------

The plug-in is by default compiled and installed to plugin/install. After this,
you can use the plug-in in your QML application by doing
"import BaaS 1.0". Note that the plugin/install directory must be in your
QML module import path.

To use Parse server into your QML
`

	import BaaS 1.0

	Parse{	
        id:backend
        hostURI:"PUT HERE YOUR SETTINGS"
        applicationId:"PUT HERE YOUR SETTINGS"
        masterKey:"PUT HERE YOUR SETTINGS"
    }
    
    BaaSModel{
        id: baasmodel
        client:backend
        endPoint:"users"
    }
    
    ListView{
    	model: baasmodel
    	..
    }
    
`

5. License
-------------------------------------------------------------------------------

See the license text file delivered with this project. 


6. Version history
-------------------------------------------------------------------------------

1.0 Initial release

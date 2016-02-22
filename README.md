BaaS QML Plug-in
==========================

This plug-in delivers an interface for Qt Quick developers for accessing the
APIs of Mobile Backend as a Service providers that allow creating mobile applications powered
by cloud services.

The plug-in is hosted at:
- 

The main features of the plug-in are:
- 

The plug-in also contains some simple examples.

The plug-in can be used as a pre-compiled shared library or you can copy the
source code into your project and compile it with your application. Although
the plug-in has been designed to be used with QML UI applications, it can also be
used as-is with Qt C++.

For more information on the implementation and usage, visit the wiki pages:
- 


1. Prerequisites
-------------------------------------------------------------------------------

 - Qt basics
 - Qt Quick basics


2. Project structure
-------------------------------------------------------------------------------

 |                  The root folder contains the project file, the license
 |                  information, and this file (release notes).
 |
 |- doc             Contains documentation on the project in general.
 |
 |- examples        Contains a set of example applications that utilise the
 |                  plug-in.
 |
 |- plugin          Root folder of the plug-in implementation.
 |  |
 |  |- src          Plug-in source codes.
 |


3. Compatibility
-------------------------------------------------------------------------------

 - .

Developed with Qt SDK 1.2.1.

3.1 Required capabilities
-------------------------


3.2 Known issues
----------------

None.


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

Visit the online project documentation and see the examples for more
information.


5. License
-------------------------------------------------------------------------------

See the license text file delivered with this project. The license file is also
available online at


6. Version history
-------------------------------------------------------------------------------

1.0 Initial release

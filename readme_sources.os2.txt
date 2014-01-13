Notes about the sources
-----------------------

The following changes made possible to build Surfer in eComStation:

1. Change the QT line in the pro file from:

   QT += webkit

   to

   QT += webkit network

2. Change the parsing of the command line arguments in "application.cpp"
   (argv[0] was parsed by the application as a command line argument, but
   it's the name of the executable).

3. Add the QT resource file "surfer.qrc" and the icons of the Oxygen theme.

4. Original source uses QIcon::fromTheme but themes aren't present in OS/2.
   I've added a fallback icon as second argument of this method:

   Original source: QIcon::fromTheme("configure");
   Changed source:  QIcon::fromTheme("configure", QIcon(":/configure.png"));

5. Other changes: OS/2 icon, OS/2 resource file...
@echo off

cd src\edu\vtc\phoebe
java -cp %ANTLR_JAR% org.antlr.v4.Tool PhoebeLexer.g4
cd ..\..\..\..


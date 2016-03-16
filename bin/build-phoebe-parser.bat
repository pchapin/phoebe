@echo off

cd src\edu\vtc\phoebe
java -Xmx2048M -cp \lib\jar\antlr-4.5.jar org.antlr.v4.Tool -visitor Phoebe.g4
cd ..\..\..\..


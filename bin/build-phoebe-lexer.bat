@echo off

cd src\org\pchapin\phoebe
java -cp ..\..\..\..\lib\antlr4-4.5.3.jar org.antlr.v4.Tool PhoebeLexer.g4
cd ..\..\..\..


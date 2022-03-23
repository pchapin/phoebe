@echo off

REM This batch file is out of date, but it is being retained as a place holder in case it might
REM prove useful in the future. Generating the Lexer should be done in IntelliJ using the ANTLR
REM plugin.

cd src\org\pchapin\phoebe
java -cp ..\..\..\..\lib\antlr4-4.9.2.jar org.antlr.v4.Tool PhoebeLexer.g4
cd ..\..\..\..


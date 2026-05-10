{$ifdef FPC} {$mode objfpc}{$H+}{$J-} {$endif}

{$apptype CONSOLE}

program MainProgram;

begin
  Writeln('Hello World');

  {* Hold the terminal in Debug mode. *}
  {$ifdef DEBUG} readln; {$endif}
end.
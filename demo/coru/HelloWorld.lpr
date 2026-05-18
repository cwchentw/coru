{$ifdef FPC} {$mode DELPHIUNICODE} {$endif}

{$ifdef WINDOWS} {$apptype CONSOLE} {$endif}

program HelloWorld;

begin
  Writeln('Hello World');

  {* Hold the terminal in Debug mode. *}
  {$ifdef DEBUG} readln; {$endif}
end.
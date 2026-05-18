{$ifdef FPC} {$mode DELPHIUNICODE} {$endif}   (*  1 *)

{$ifdef WINDOWS} {$apptype CONSOLE} {$endif}  (*  2 *)

program HelloWorld;                           (*  3 *)

begin                                         (*  4 *)
  Writeln('Hello World');                     (*  5 *)

  {* Hold the terminal in Debug mode. *}      (*  6 *)
  {$ifdef DEBUG} readln; {$endif}             (*  7 *)
end.                                          (*  8 *)

clear;
if [ -f "$1" ]; then
  ./binstr2hex <  "$1"| ./vsm
fi

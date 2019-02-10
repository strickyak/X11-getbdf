X11 getbdf

From http://www.gnu-darwin.org/distfiles/getbdf.tar.Z

Hints:
```
make

./getbdf -font \
  -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1 \
  > FONT_-adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1.bdf
```

*   I enabled the nint() function.
*   Works for me on `x86_64 GNU/Linux`.
*   Ignore all the warnings from gcc.

`github.com/strickyak/X11-getbdf`

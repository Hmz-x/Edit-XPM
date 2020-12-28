# Edit-XPM
Edit XPM files via CLI. Uses no GUI, designed for 
using within scripts/programs or just from a shell.
Written by Hamza Kerem Mumcu (hamzamusic34@gmail.com)

## Compiling

Using the make build tool you can easily compile the project by typing:

    $ make

## Usage
    
    $ ./ed-xpm [OPTION] <FILE> <FILTER...>

    $ ./ed-xpm --inv=100 <xpm_file>                  # Invert RGB colors of xpm image file %100
    $ ./ed-xpm --flp=t img.xpm                       # Flip top half of xpm image file
    $ ./ed-xpm -c --inv=20 --sat=5 img.jpg           # Convert image to xpm, then apply inversion and saturation filter
    
    -c: convert image file to .xpm format using 'convert'
    <FILE>: image file to be processed
    <FILTER>: filter to apply to image (can use multiple back to back)
    Available filters: '--inv=<range>' (inverted colors by possibility, range: 0-100), '--sat=<range>' (saturate, range:-100-100),
    '--flp=<side>' (flip given side, side: t/b for top/bottom).
    Filters can be chained back to back in order to apply processing one after the other.

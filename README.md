# ProximityBand-Arduino

## Usage:

<b>Setting the LEDs brightness:</b><br>
While the device is disconnected, press and hold the button.<br>
All three LEDs will light up, set the switch to position 1-2-3.<br>
Position 1: Min LED brightness.<br>
Position 2: Medium LED brightness.<br>
Position 3: Max LED brightness.<br>
Let go of the button and the change is saved to memory.<br>
The brightness is retained even after the device is turned off.<br>

<b>Setting silent / vibrate / sound mode:</b><br>
While the device is connected, set the switch to position 1-2-3.<br>
Position 1: Silent mode.<br>
Position 2: Vibrate mode.<br>
Position 3: Sound mode.<br>

<b>Setting do-not-disturb mode:</b><br>
While the device is connected, press and hold the button.<br>
Set the switch to position 1 to turn on DnD mode.<br>
Set the switch to position 2 or 3 to turn off DnD mode.<br>
Let go of the button.<br>

<b>Turning off the alarm:</b><br>
In any case while the alarm is turned on, simply press the button.<br>
This will stop the vibration and light flashing on the Proximity Band as well as the Android device (if it is still connected).<br>

## TODO:
1. Make the LEDs flash in unison or some other pattern instead of on their own induvidual timers.
2. Make the motor pulse in some preset pattern.
3. Make the LEDs fade in and out.

## Technical details:
<pre>
<b>Determining if the Bluetooth connection is active:</b>
If any data via BLE is recieved, a timer is set for 1 second.
If no other data is recieved after the timer is done, this means that the connection is not active.
In this case the alarm will be acivated.

<b>Connection to Bluetooth module using pins:</b>
RX (Arduino) 	= 	D11
TX (Arduino) 	= 	D12
CST (Arduino) 	= 	D2

<b>Getting data from Bluetooth UART:</b>
Red LED On Command         		=	'R'
Green LED On Command       		=	'G'
Blue LED On Command        		=	'B'
Red LED Off Command        		=	'r'
Green LED Off Command      		=	'g'
Blue LED Off Command       		=	'b'

Red LED Flash On Command    	        =       'I'
Green LED Flash On Command  	        =       'J'
Blue LED Flash On Command   	        =       'K'
Red LED Flash Off Command   	        =       'i'
Green LED Flash Off Command 	        =       'j'
Blue LED Flash Off Command  	        =       'k'

Motor Short Command      	 	=	'1'
Motor Medium Command     	 	=	'2'
Motor Long Command       	 	=	'3'

Alarm On Command           	 	=	'X'
Alarm Off Command           	        =    	'x'
Alarm Supress Command                   =	'Y'

Acknowledged Command                    =	'A'

<b>Sending data to Android using Bluetooth UART:</b>
Switch Position 1 Command  		=	"S1"
Switch Position 2 Command  		=	"S2"
Switch Position 3 Command  		=	"S3"

Button Pressed Command    		=	"B1"
Button Unpressed Command  		=	"B0"
</pre>

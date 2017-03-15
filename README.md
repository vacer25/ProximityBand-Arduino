# ProximityBand-Arduino

<pre>
<b>Connection to Bluetooth module using pins:</b>
RX (Arduino) 	= 	11
TX (Arduino) 	= 	12
CST (Arduino) 	= 	2

<b>Getting data from Bluetooth UART:</b>
Red LED On Command         		=	R
Green LED On Command       		=	G
Blue LED On Command        		=	B
Red LED Off Command        		=	r
Green LED Off Command      		=	g
Blue LED Off Command       		=	b

Red LED Flash On Command    	=	I
Green LED Flash On Command  	=	J
Blue LED Flash On Command   	=	K
Red LED Flash Off Command   	=	i
Green LED Flash Off Command 	=	j
Blue LED Flash Off Command  	=	k

Motor Short Command      	 	=	1
Motor Medium Command     	 	=	2
Motor Long Command       	 	=	3

Alarm Command           	 	=	X

<b>Sending data to Android using Bluetooth UART:</b>
Switch Position 1 Command  		=	S1
Switch Position 2 Command  		=	S2
Switch Position 3 Command  		=	S3

Button Pressed Command    		=	B1
Button Unpressed Command  		=	B0

<b>Determining if the Bluetooth connection is active:</b>
If any data is recieved, a timer is set for 30 second (will be chaged to 3 seconds).
If no other data is recieved after the timer is done, this means that the connection is not active.
In this case the alarm will be acivated.
</pre>
<pre>
<b>TODO:</b>
Make the LEDs flash in unison or some other pattern instead of on their own induvidual timers.
Make the motor pulse in some preset pattern.
</pre>

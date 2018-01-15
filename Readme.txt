 https://forum.arduino.cc/index.php?topic=117009.0
 
 
 
 //Plays synchronously an audio file. Busy pin is used for this method.
 wtv020sd16p.playVoice(0);
 //Plays asynchronously an audio file.
 wtv020sd16p.asyncPlayVoice(1);
 //Plays audio file number 1 during 2 seconds.
 delay(5000);
 //Pauses audio file number 1 during 2 seconds.  
 wtv020sd16p.pauseVoice();
 delay(5000);
 //Resumes audio file number 1 during 2 seconds.
 wtv020sd16p.pauseVoice();
 delay(5000);  
 //Stops current audio file playing.
 wtv020sd16p.stopVoice();
 //Plays synchronously an audio file. Busy pin is used for this method.  
 wtv020sd16p.asyncPlayVoice(2);
 delay(2000);  
 //Mutes audio file number 2 during 2 seconds.
 wtv020sd16p.mute();
 delay(2000);
 //Unmutes audio file number 2 during 2 seconds.
 wtv020sd16p.unmute();
 delay(2000);    
 //Stops current audio file playing.
 wtv020sd16p.stopVoice();
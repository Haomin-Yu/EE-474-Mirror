/*
 * Function name: EKGNormalizer
 * Function inputs: This file inputs the data given about the EKG waveform.
 * Function outputs: This outputs data normalized to account for any noise and inconsistency so the outputted data displayed and stored
 *                   will be closer to its actual value.
 * Function description: This function is used to normalize data so it will be more close to the actual ekg wave data since it comes into 
 *                       the system a little off consistantly and can be changed to be closer to the actual value that should be inputted.
 * Author: Haomin Yu and Nathan Ness
 */
double ekgNormalizer(double peak) {
  //This is used to normalize the data between 100 and 1090 Hz.
	if(peak > 107.9 && peak < 1089.6) {
	  //return peak*0.9164 + 2.414;
   return peak*0.884 + 2.414;
	}
 //This is used to normalize the data between 1090 and 3265 Hz.
	else if(peak >= 1089.6 && peak < 3265.0) {
	  //return peak*0.919 - 0.4676;
    return peak*0.882 + 5.4676;
	}
 //This is used to normalize the data greater than 3265 Hz.
	else if(peak >= 3265.0) {
	  //return peak*0.916 + 11.246;
    return peak*0.88 + 11.246;
	}
 //This is used to normalize the data less than 100 Hz.
  else {
    return peak*0.88;
 }
}

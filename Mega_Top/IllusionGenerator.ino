
double illusionGenerator(double peak) {
	if(peak > 107.9 && peak < 1089.6) {
	  return peak*0.9164 + 2.414;
	}
	else if(peak >= 1089.6 && peak < 3265.0) {
	  return peak*0.919 - 0.4676;
	}
	else if(peak >= 3265.0) {
	  return peak*0.916 + 11.246;
	}
}
//John Henry Buenaventura
//
void sliding(int slide, int sprite_y)
{
    if(slide > 32 && slide < 0){
	sprite_y += 1;
	slide--;
    }
    else if(slide <= 32 && slide > 50){
	sprite_y -= 1;
	slide--;
    }
    else{
	slide = 0;
	sprite_y = 75;
    }

}

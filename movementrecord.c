




GameProperties read_data(char* filename){
	GameProperties gp
}

void write_data(char* filename, GameProperties gp){
	FILE *f = fopen(filename, "wb");
	fwrite(seed, sizeof(int), 1, f);
	fwrite(pen_num, sizeof(int), 1, f);
	fwrite(movements, sizeof(bool), sizeof(movements), f);
	fclose(f);
}

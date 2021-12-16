#include "pandemic.h"
#include <stdio.h>
#include <ctype.h>

// find the index of the comma, which can judge the ilegal input
int comma_num_index(const char* line){
  int comma_num = 0;
  int comma_index = 0;
  int current_index = 0;

  while(line[current_index] != '\0'){
    if(line[current_index] == ','){
      comma_num += 1;
      comma_index = current_index;
    }

    current_index++;
  }

  // if the comma_num isn't 1, return -1, 
  //which means the input is ilegal(no comma or has commas more than 1).

  if(comma_num != 1){
    if(comma_num == 0){
      printf("Ilegal input(no comma).\n");
    }
    else{
      printf("Ilegal input(more than 1 commas).\n");
    }
    exit EXIT_FAILURE;
  }
  // if the comma_num is 1, return the index of comma.
  return comma_index;
}

// check the num of name is legal. if legal, return the lenth of NAME.
int name_num(const char* line, int comma_index){
  // if the 'name' is null, comma_index = 0. [ilegal]
  if(comma_index == 0){
    printf("There is error in 'NAME', its lenth is 0.\n");
    return EXIT_FAILURE;
  }
  // if the num of 'name' is larger than the size of name_array(64) [ilegal]
  if(comma_index >= 64){
    printf("There is error in 'NAME', it's too long.\n");
    exit EXIT_FAILURE;
  }

  // if the num of 'name' is legal, return the lenth of 'name'
  return comma_index;
}

// check whether the 'population' is made up of pure numbers, return its legal lenth.
int popu_check(const char* line, int comma_index){
  int popu_index = comma_index + 1;
  int popu_len = 0;

  while(line[popu_index] != '\0' && line[popu_index] != '\n'){
    // pure number?
    if(line[popu_index] < '0' || line[popu_index] > '9'){
      printf("There is not pure numbers in POPULATION'.\n");
      exit EXIT_FAILURE;
    }
    popu_len++;
    popu_index += 1;
  }

  // if it's not pure numbers or the lenth of 'population' is 0, 
  // return 'ilegal' label.
  if(popu_len == 0){
    printf("There is error in the input of 'Population'.\n");
    exit EXIT_FAILURE;
  }

  // if the 'population' is legal, return its lenth.
  return popu_len;
}

void get_name(const char* line, char* name, int comma_index){
  int name_len = name_num(line, comma_index);
  // ----get name----
  int i;
  for(i = 0; i < name_len; i++){
    name[i] = line[i];
  }
  name[i] = '\0';
}

int get_population(const char* line, int comma_index){
  // ----get population---
  int current_number = 0;
  int population_index = comma_index + 1;
  int population_len = popu_check(line, comma_index);
  int population_final = 0;
  
  // save every number into array(char -> int)
  for(int i = 0; i < population_len; i++){
    current_number = line[population_index + i] - '0';

    for(int j = 1; j < (population_len - i); j++){
      current_number = current_number * 10;
    }
    population_final += current_number;
  }
  return population_final;
}

country_t parseLine(char * line) {
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;

  // get the index of comma
  int comma_index = comma_num_index(line);
  // get the name_info
  get_name(line, ans.name, comma_index);
  // get the number of population
  ans.population = get_population(line, comma_index);
  return ans;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  int sum = 0;
  int day_num = n_days - 7 + 1;

  for(int i = 0; i < day_num; i++){
    sum = 0;
    for(int j = 0; j < 7; j++){
      sum += data[i+j];
    }
    avg[i] = (double)sum / (double)7;
  }
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  int sum = 0;
  for(int i = 0; i < n_days; i++){
    sum += data[i];
    cum[i] = (double)sum * (double) 100000 / (double)pop;
  }
}

// find the max value in the array
unsigned int max_case(unsigned *data, size_t n_days){
  unsigned int max_val = 0;
  for(int i = 0; i < n_days; i++){
    if(data[i] > max_val){
      max_val = data[i];
    }
  }
  return max_val;
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  //WRITE ME
  int current_val = 0;
  int max_case_val = 0;
  int max_case_label = 0;
  for(int i = 0; i < n_countries; i++){
    current_val = max_case(data[i], n_days);
    if(current_val >= max_case_val){
      max_case_val = current_val;
      max_case_label = i;
    }
  }
  printf("%s has the most daily cases with %u\n", countries[max_case_label].name, max_case_val);
}

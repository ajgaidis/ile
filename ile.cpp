//Copyright (C) 2019 Jon Pry
//
//This file is part of Pill.
//
//Pill is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 2 of the License, or
//(at your option) any later version.
//
//Pill is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Pill.  If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>  // Imports integer types etc.
#include <stdio.h>   // Imports I/O functions like printf() and fopen()
#include <stdlib.h>  // Import standard library functions like exit()

// 1D array of 8-bit values
uint8_t key[] = {0x36, 0x61, 0x3e, 0xf1, 0x69, 0x3c, 0x2e, 0x8e, 0x55, 0xcf, 0xc2, 0x86, 0x9c, 
                 0x3b, 0x94, 0x22, 0x64, 0x41, 0x0b, 0x6b, 0xe5, 0xac, 0xa9, 0x19, 0xd3, 0xd0,
                 0xaa, 0x9e, 0x49, 0x05, 0x03, 0x7f, 0x67, 0x42, 0x72, 0x50, 0xfd, 0xa0, 0x5e,
                 0x53, 0xef, 0x21, 0x59, 0x0c, 0x5c, 0xee, 0xae, 0xc0, 0xaf, 0xb9, 0xab, 0x15,
                 0xe6, 0xd4, 0x2e, 0xba, 0xa6, 0x59, 0x59, 0x6f, 0x5e, 0x5d, 0xef, 0x45, 0x1f,
                 0xe1, 0x95, 0x9c, 0x02, 0xf4, 0x70, 0x72, 0x95, 0xc9, 0xfd, 0xf2, 0xb8, 0xac,
                 0x33, 0xe7, 0x67, 0x5f, 0xfd, 0xcd, 0x34, 0x2c, 0x88, 0x5a, 0x85, 0x62, 0xca,
                 0x64, 0x3f, 0xba, 0xa9, 0xdd, 0x1c, 0x40, 0x7a, 0x9d, 0xb4, 0x6a, 0x10, 0x4a,
                 0x35, 0x0f, 0xbc, 0x6d, 0xbb, 0x70, 0x56, 0xa2, 0xcf, 0x54, 0x70, 0x04, 0x00,
                 0x79, 0x5f, 0x86, 0x5b, 0xa9, 0xea, 0x9a, 0xe3, 0x14, 0xf7};


int main(int argc, char** argv){
   //Load entire file into memory

   // The conditional below checks that there is 1 argument, the file to
   // decrypt (it says != 2 here because the program name is an implicit
   // argument)
   if(argc != 2){  
      printf("Usage: ./decrypt FILENAME\n");
      exit(0);
   }

   // Open the file passed as the first argument (argv[1]) to the program with
   // read-only permissions ("r")
   FILE* f = fopen(argv[1],"r");  
   if(!f){  // Check that the file opened correctly, else fail/exit
       printf("Could not open file %s\n", argv[1]);
       exit(0);
   }

   // The next three lines are a trick to get the size of the file.
   // Essentially, you set the file position to the end of the file (fseek()),
   // read its position (ftell()) which is its size, and then reset the file
   // position to the beginning of the file.
   fseek(f,0,SEEK_END);
   size_t sz = ftell(f);
   fseek(f,0,SEEK_SET);

   // Allocate a buffer of size sz. They use valloc here to ensure the buffer
   // is page aligned though I don't think that is really necessary.
   uint8_t *buf = (uint8_t*)valloc(sz);

   // Copy the file (f) with size sz into the buffer we just created.
   fread(buf,sz,1,f);

   // Close the file since we don't need it anymore
   fclose(f);

   // Loop with j=62. After that j will be 63 and the loop will break... so IDK
   // why they do a loop here...
   for(int j=62; j < 63; j++){

   // Loop with k=0. Again, k will only ever be 0, so IDK why they loop.
   for(int k=0; k < 1; k++){

      // Get the size of the key in bytes (essentially just the length of the
      // key array (since each item is 1 byte))
      int ki=sizeof(key)-1-j;

      // Initialize vstate
      uint8_t vstate = 0;

      // Loop where i will take on all byte positions in the key array (since
      // k=0).
      for(size_t i=0; i < sz-k; i++){
         if(i>64)
            // They XOR vstate with a byte in the buffer (file data) at index i
            // and XOR that result with a byte in the key at index ki. Then,
            // they print the result as a character (%c).
            printf("%c", vstate ^ buf[i+k] ^ key[ki]);
         // Update values
         vstate = buf[i+k];        
         ki--;
         if(ki<0)
           ki = sizeof(key)-1;
      }
   }}
}


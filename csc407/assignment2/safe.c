#include "safeBustersHeaders.h"

int runProgram = 1;

int safeOpened = 0;

int firstNumber;

int secondNumber;

int thirdNumber;

int computeResponse(int* countPtr){

  int toReturn;
  
  sleep(1);
  (*countPtr)--;
  
  if(*countPtr == 0){
    printf("Safe \"Click!\"\n");
    toReturn = SIG_RIGHT_DIGIT;
  }
  else{
    printf("Safe \"Nope.\"\n");
    toReturn = SIG_WRONG_DIGIT;
  }

  return(toReturn);

}

void simpleHandler (int sigInt){

  runProgram = 0;

}

void advancedHandler (int sigInt, siginfo_t* infoPtr, void* dataPtr){

  if(firstNumber > 0){
    int i = computeResponse(&firstNumber);
    kill(infoPtr->si_pid, i);
  }
  else if(secondNumber > 0){
    int i = computeResponse(&secondNumber);
    kill(infoPtr->si_pid, i);
  }
  else  if(thirdNumber > 0){
    int i = computeResponse(&thirdNumber);
    if(i == SIG_RIGHT_DIGIT){
      safeOpened = 1;
    }
    kill(infoPtr->si_pid, i);
  }

}

int main (int argc, char* argv[]){
  
    srand(getpid());

    struct sigaction act;

    firstNumber = (rand()%16)+1;
    secondNumber = (rand()%16)+1;
    thirdNumber = (rand()%16)+1;
  
    memset(&act, '\0', sizeof(act));
    act.sa_handler = simpleHandler;
    sigaction(SIG_QUIT, &act, NULL);
    
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = advancedHandler;
    sigaction(SIG_TRY_NEXT_DIGIT, &act, NULL);

    printf("The combination is %d %d %d", firstNumber, secondNumber, thirdNumber);
  while(runProgram)
    sleep(1);
  if(safeOpened)
    printf("Something humble");
  else
    printf("Some bragging message");

  return(EXIT_SUCCESS);
}


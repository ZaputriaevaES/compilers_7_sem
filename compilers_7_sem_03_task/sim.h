#define SIM_X_SIZE 512
#define SIM_Y_SIZE 256

#ifndef __sim__
#ifdef __cplusplus
extern "C" {
#endif

void simInit();
void app();
void simExit();
void simFlush();
void simPutPixel(int x, int y, int argb);
int simRand();

#ifdef __cplusplus
}
#endif
#endif
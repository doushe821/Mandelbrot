int main()
{
    const int ScreenX = 800;
    const int ScreenY = 800;
    int PixelSet[ScreenX * ScreenY] = {};
    const int N = 255;
    const double stepX = 0.003;
    const double stepY = 0.003;
    const double BorderCircumferenceRadiusSquare = 3;

    const int CenterX = ScreenX/2;
    const int CenterY = ScreenY/2;
    
    for(int yPixels = 0; yPixels < ScreenY; yPixels++)
    {
        double Y = ((double)(CenterY - yPixels)) * stepY;
        double y0 = Y;
        for(int xPixels = 0; xPixels < ScreenX; xPixels++)
        {
            double X = ((double)(xPixels - CenterX)) * stepX;
            Y = ((double)(CenterY - yPixels)) * stepY;
            double x0 = X;
            int Iterations = 0;
            for(; Iterations < N; Iterations++)
            {
                double xx = X*X;
                double yy = Y*Y;
                double xy = X*Y;

                if(xx + yy > BorderCircumferenceRadiusSquare)
                {
                    break;
                }

                X = xx - yy + x0;
                Y = 2 * xy + y0;
            }
            PixelSet[yPixels * ScreenX + xPixels] = Iterations;
        }
    }

    FILE* DebugFile = fopen("dbf.txt", "w+b");
    for(unsigned i = 0; i < ScreenX * ScreenY; i++)
    {
        fprintf(DebugFile, "%d ", PixelSet[i]);
    }
    fclose(DebugFile);
    ErrorParser(DisplayPixelsSDL(PixelSet, ScreenX, ScreenY));

    return 0;
}

// ソーベルフィルター
float4 Sobel(float2 tex, Texture2D depthBuffer)
{
	const float threshold = 0.9;//0.05;

	const int2 texAddrOffsets[8] = {
			int2(-1, -1),
			int2(0, -1),
			int2(1, -1),
			int2(-1,  0),
			int2(1,  0),
			int2(-1,  1),
			int2(0,  1),
			int2(1,  1),
	};

	float lum[8];
	int i;

	float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };

	uint width, height, levels;

	depthBuffer.GetDimensions(0, width, height, levels);

	for (i = 0; i < 8; i++) {
	  float3 colour = depthBuffer.Load(int3(int2(tex * float2(width, height)) + texAddrOffsets[i], 0));
	  lum[i] = dot(colour, LuminanceConv);
	}

	float x = lum[0] + 2 * lum[3] + lum[5] - lum[2] - 2 * lum[4] - lum[7];
	float y = lum[0] + 2 * lum[1] + lum[2] - lum[5] - 2 * lum[6] - lum[7];
	float edge = sqrt(x*x + y * y);

	 return float4(edge, edge, edge, 1);
}
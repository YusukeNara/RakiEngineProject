#include "Sprite.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	//出力用変数
    GSOutput output;
	
	//縦横幅をもとに出力オフセットを作成
    float2 sizeOffset[4] =
    {
        { 0              , input[0].size.y },	//左下
        { 0              , 0 },					//左上
        { input[0].size.x, 0 },					//右下
        { input[0].size.x, input[0].size.y },	//右上
    };
	
	//点の入力から四角形を出力
	for (uint i = 0; i < 4; i++)
	{
		//出力頂点のローカル座標を、描画サイズに調整
        output.pos = input[0].svpos.xy + sizeOffset[i];
		//ローカル座標をワールド座標に変換
        output.pos = mul(input[0].ins_matrix, output.pos);
		
	}
}
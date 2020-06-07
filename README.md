# DirectX 3D 
Resources : http://www.rastertek.com/dx11s2tut04.html       

## Index Buffers
To record the location of each vertex that is in the vertex buffer.


## Vertex Buffers
Mainly used for transforming the vertices from the vertex buffer into 3D space.
The vertex shader program will be called by the GPU for each vertex it needs to process.

## Constant Buffers
It contain shader constant data. It enables user to supply shader constants data to the pipeline.
I can use a constant buffer to store the 
### Create a constant buffer.
1. Define a structure that describtes the vertex shader constant data.
2. Allocate memory for the structure that I defined in step 1. Fill this buffer with vertex shader
constant data. 
```


```




## Graphic pipeline process
1. Create Swap chain.
2. Create Render Target View.
3. OMSetRendertargets
4. ViewPort
5. Compile the vertex shader 
6. Create Vertex shader.
7. Define the imput layout



Resources: 
https://bell0bytes.eu/the-swap-chain/      
### Swap Chain.
-> Used for avoiding screen tearing. 
Double buffering is used to display a image or frame while a seprate frame is being buffered 
to be shown next. 

#### DXGI 
This implements a feature - swapping, page flipping.     
it swaps the back buffer and the displace surface. DirectX uses a pointer for each buffer and simply
switches their values.

Resources: 
https://docs.microsoft.com/en-us/windows/desktop/direct3d11/d3d10-graphics-programming-guide-output-merger-stage     
### Output Merger Stage.
The OM stage generates the final rendered pixel color using a combination of pipeline state.
This is the final step for determining which pixels are visible and blending the final pixel colors.


#### Depth Stencil Testing.
The Depth data is used to determining which pixels lie closest to the camera.



### Rasterizer

#### If I want to do...Skeleton of shape
```
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	hr = dev->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState);
	if (FAILED(hr))
	{
		return ERROR;
	}
	devcon->RSSetState(g_RasterizerState);

```


## Texturing
### Necessary Thing.

```
ID3D11ShaderResourceView* CubesTexture;
ID3D11SamplerState* CubesTexSamplerState;
```
1. Define a set of texture coordinates to object. 
- Texture coordinates are defined for each vertex of geometry and the texture coordinate are going to decide
which part of the texture to sample.      
In order to do this, I need to do UV mapping.(which is 2D Texture)....        
#D textures -> uses an extra "w" value.     
https://en.wikipedia.org/wiki/UV_mapping      
#### **UV mapping**       
*This is the 3d modeling process of projecting a 2D image to a 3D model's surface for texture mapping.*        

https://en.wikipedia.org/wiki/Mipmap        
#### Mip Mapping
Mip mapping is the process of generating a chain of images from a texture where each image in the chain is half the size of the previous image in the mipmap chain.     
##### Advantages
* Level Of Details(LOD)
* Improve image quality
* Speeding up rendering times.

**LOD?**        
If the textured object go further away from the camera, less detail in the object's texture is visible. So, full resolution texture is not needed to render the object.          




#### Texture Sampler           




* vID3D11SamplerState* CubesTexSamplerState;         
The texture sampler state defines how a texture from the texture is read.                
Assume ID3D11ShaderResourceView*             g_pTextureRV = nullptr;        
```
hr = CreateDDSTextureFromFile( g_pd3dDevice, L"floor.dds", nullptr, &g_pTextureRV );
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );

```
* minLOD - The lowest mpmap level to use. 0 is the most detailed and largest one.

Different way.

```
	DirectX::ScratchImage image;
	result = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
	result = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_texture);
```
##### How to get JPG file?
--> Use CreateWICTextureFromeFile...
##### How to use LoadFromDDSFile and CreateShaderResourceView?

1. Go to Microsoft/DirectXTex
2. Build it. 
3. Copy paste ... DirectXTex.h, DirectXTex.inl, DirectXTex.lib


##### What is the difference in the rendering part?
We need to send the sampler state and texture to the pixel shader only onece per group of ojects.
PSSetShaderResources() an PSSetSampler().... 


#### Draw Font     
source: https://docs.microsoft.com/en-us/windows/desktop/direct3darticles/surface-sharing-between-windows-graphics-apis      
##### Surface Sharing
Direct3D 11, Direct3D 10.1, Direct2d - interoperability with each other.
These things are able to interoperate with each other using either Direct3D 10.1 device sharing or
synchronized shared surfaces.       

Device Sharing is appropriate for in-process, single-threaded usage of one rendering device shared by
both Direct3D 10.1 and Direct 2D rendering APIs.


##### What is D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX????       


##### Back face meaning?
source: https://stackoverflow.com/questions/17549145/directx-11-frontface-direction       
```
rasterDesc.CullMode = D3D11_CULL_BACK;
rasterDesc.FrontCounterClockwise = true;
```
* Tell D3D to cull the back face.
* Define my vertex in counter-Clockwise for every face.

WHich means that user sees every faces rendered well because they were treated as front face.
Then change *frontCountClockwise* to false.

```
rasterDesc.FrontCounterClockwise = false;
```
- Tell d3d the faces which defined in counter-clockwise is back face.
so D3d culls all of the faces, thus nothing was rendered.

FrontCounterClockwise = true
- Faces were treated as front face if it's vertex order is counter-clockwise
- faces were treated as back face if it's vertex order is clockwise. 





# HLSL
Source: https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-signatures           
A shader signature is a list of the parameters that are either input to or output from a shader function.
Shader signatures are used for binding shader outputs with inputs without the overhead of semantic resolution.

## Semantics
A semantic is a string attached to a shader input or output that conveys information about the intended
use of a parameter. In other words, it is a name used to specify how data is passed from a part of the graphics pipeline to another.

Vertex shaders use semantics to link data from the vertex buffers sent by the app, while
pixel shaders use semantics to link data from the outputs of a vertex shader.

## Vertex shader
* Transforming vertices

## Pixel shader
* Determining the final color of a pixel on the screen.

Pixel shader output is the computed pixel color.

### Shader Innputs




The sampler is used to read the texture color on the corresponding texture coordinates.




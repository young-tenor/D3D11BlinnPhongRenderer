# D3D11 Blinn-Phong Renderer
D3D11로 진행한 Blinn-Phong 셰이딩 기반의 프로젝트입니다. 엔진 프로그래밍보다는 그래픽스 알고리즘 구현에 집중했습니다. 굳이 PBR이 필요하지 않은 알고리즘을 다룹니다.

아래처럼 main 함수에서 어떤 알고리즘을 검사할지 결정할 수 있습니다.

```cpp
std::unique_ptr<App> app;

//app = std::make_unique<BlinnPhong>();
//app = std::make_unique<Billboard>();
//app = std::make_unique<Tessellation>();
app = std::make_unique <Mipmap>();
```
### 목차
* [Blinn-Phong](#blinn-phong)
* [Billboard](#billboard)
* [Tessellation](#tessellation)
* [Mipmap](#mipmap)

## Blinn-Phong
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/9ea8843a-c65f-4bce-8fdf-497834cf0fa7" />

## Billboard
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/77b247f3-1306-4778-b147-2a6dee5c6bfc" />

## Tessellation
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/fec5b9b6-8b59-4fbc-907f-39853447fcb9" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/c2142055-b02b-4269-acae-302f5d32d358" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/07b4fe45-1f19-401a-ac09-b5caa84f1174" />

구에 subdivision을 적용하고 완전히 가려지는 부분은 컬링합니다.

## Mipmap
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/9fd852d0-2c26-4616-983f-29f962d0cbb1" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/db0ac400-e648-4152-accd-f46e424e313b" />

## Credits
* **f-texture**: Provided by [WebGL Fundamentals](https://webglfundamentals.org/), licensed under [BSD 3-Clause License](https://github.com/gfxfundamentals/webgl-fundamentals/blob/master/LICENSE).
* **Brick Texture**: Provided by [ambientCG](https://ambientcg.com/), licensed under CC0 1.0 Universal.
* **Tree Texture**: Created by [TransparentPNG](https://www.transparentpng.com/), licensed under CC BY 4.0.

# SuperPacMan - Luong Thanh Vinh Edition

- Thử nghiệm game: [https://youtu.be/]

# Giới thiệu game

SuperPacman là tựa game mà bạn phải tự mình điều khiển Pacman di chuyển trong vũ trụ sao cho
vừa tránh né những thiên thạch rơi xuống, vừa phải ăn những đồng xu và sinh tồn càng lâu càng
tốt

- [0. Cách tải game](#0-cách-tải-game)
    * [Bao gồm code và có thể biên dịch.](#a-bao-gồm-code-và-có-thể-biên-dịch)
- [1. Bắt đầu game](#1bắt-đầu-game)
- [2. Chọn Mode](#2chọn-Mode)
- [3. Các thành phần trong game:](#3-các-thành-phần-trong-game)
- [4. Cách chơi](#4-cách-chơi)
    * [a. Các vật thể trong game](#a-các-vật-thể-trong-game)
- [5. Game Over](#5-Game-over)
- [Về đồ họa của game:](#về-đồ-họa-của-game)
- [Về source code game](#về-source-code-game)

# 0. Cách tải game


## Bao gồm code và có thể biên dịch.

**Bước 1:** Clone repo này về. <br/>
Hoặc Chọn Code -> Download Zip (Khoảng 90MB)

![image](Input/HuongDan/1.png)

- Có thể nhảy đến bước 5 để chơi luôn.

**Bước 2:** Cài mingw. Ở đây tôi cài đặt phiên bản MinGW-W64
GCC-8.1.0 [x86_64-posix-seh](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z/download). Ngoài ra các bạn có thể tìm kiếm và tải các bản mới hơn nếu có.<br/>
Một số chú ý: <br/>

- Phải xóa ba dll của c++ trong thư mục chứa pvz.exe: `libgcc_s_seh-1.dll`, `libstdc++-6.dll`, `libogg-0.dll`. Ba dll này chỉ dành cho người chơi không cài sẵn c++ trong máy.

**Bước 3:** Cài GNU make

**Bước 4:** Mở cmd trong thư mục chứa Makefile và gõ lệnh make hoặc mingw32-make.

**Bước 5:** Bật main.exe và chơi

# 1. Bắt đầu game

Chờ một chút rồi click vào nút “CLICK TO START!” để bắt đầu chơi game.
<div style="text-align: center;">

![ezgif com-video-to-gif](resources/preview/loading.gif)

</div>


Khi vào game lần đầu, người chơi hãy nhập tên mình để bắt đầu trò chơi. Sau đó click “OK” hoặc nhấn phím Enter để tiếp
tục.

<div style="text-align: center;">

![image](resources/preview/name.png)

</div>



Và người chơi sẽ ngay lập tức nhận được 1 cây đầu tiên - Peashooter
<div style="text-align: center;">

![image](resources/preview/first_plant.png)

</div>

# 2. Chọn level

Có tổng cộng 12 level với mức độ khó dễ khác nhau để bạn chơi. Khi di chuột lên 1 level, level đó sẽ phát sáng. Chỉ cần
click chuột để chơi level đó.
<div style="text-align: center;">

![image](resources/preview/choose_level.png)
</div>


Tuy nhiên khi mới bắt đầu, bạn chỉ mở được 1 level. Chiến thắng lần lượt từng level để mở khóa và chơi level tiếp theo:
<div style="text-align: center;">

![image](resources/preview/choose_level1.png)
</div>


Ngoài ra còn có 3 nút chức năng khác để người chơi đổi tên, đóng game hay thậm chí là reset lại tiến trình của game để
trải nghiệm game lại từ đầu.

<div style="text-align: center;">

![image](resources/preview/choose_level2.png)
</div>



Khi vào game, có 2 loại level mà mọi người sẽ gặp: buổi sáng (bên trái) và buổi tối (bên phải)
<div style="text-align: center;">

![image](resources/preview/levels.png)
</div>

- Buổi sáng thì thi thoảng sẽ có sun (mặt trời) từ trên trời rơi xuống.
- Còn buổi tối thì không có, vậy nên game sẽ khó hơn một chút.

Trước khi bắt đầu game, bạn sẽ được biết trước có những loại zombie gì sẽ xuất hiện.
<div style="text-align: center;">

![image](resources/preview/credit.png)
</div>

# 3. Các thành phần trong game:

-    Sun (Mặt trời) ![image](resources/preview/sun.png)
     : là đơn vị để mua plant (cây) trong game. Nhấp vào để thu. Và nên chú ý là nếu một sun tồn tại quá lâu, nó sẽ mờ
     dần rồi biến mất. Bạn sẽ không thể thu chúng sau khi chúng đã biến mất.

-    Sun bar (Thanh mặt trời) ![image](resources/preview/sun_bar.png)
     : là bảng đếm số mặt trời mà người chơi đang có

- Plant seeds: tất cả các giống cây là người chơi đang sở hữu. Để chọn một giống cây mang đi trồng, người chơi phải
  click vào plant seed hoặc nhấn phím 1, 2, 3, 4, 5 (như trong game đánh dấu) để chọn cây rồi trồng trên sân. Tất nhiên,
  bạn phải đủ sun để mua cây và plant seed phải có thời gian chờ khác nhau để trồng cây giống đó lần nữa.

<div style="text-align: center;">

![image](resources/preview/plant_seed.png)
</div>

- Shovel (Xẻng): Là vật dụng vô cùng hữu hiệu để bạn có thể đào đi những cây không cần thiết và thay vào đó cây mới.

<div style="text-align: center;">

![image](resources/preview/shovel.png)
</div>

- Thanh tiến trình game (progress bar): Là thanh bạn có thể theo dõi để xem còn bao nhiêu zombie tiếp theo sẽ ra hay bạn
  đã chơi được bao nhiêu phần của level. Bên trái sẽ là level hiện tại bạn đang chơi.

<div style="text-align: center;">

![image](resources/preview/progress_bar.png)
</div>

- Nút tua nhanh game (Turbo button): Nếu thấy game quá chậm hay quá dễ mà bạn cần tiết kiệm thời gian, hãy nhấn nút để
  trải nghiệm game với tốc độ x2 tốc độ gốc.
- Và nếu thấy game quá đáng sợ ở tốc độ này, hãy nhấn lại để game trở về tốc độ bình thường.
  ● Game ở tốc độ ban đầu:  ![image](resources/preview/faster.png)

● Game đang ở tốc độ nhanh gấp 2 ban đầu:  ![image](resources/preview/faster_triggered.png)

- Nút dừng game (Pause Button): Bạn có việc cần dừng game để lúc sau chơi tiếp ư? Hay bạn muốn điều chỉnh âm lượng của
  nhạc nền và hiệu ứng âm thanh? Quá đơn giản, game có thể dừng lại bằng cách click ![image](resources/preview/pause.png)  hoặc
  click đâu đó bên ngoài game.
- Máy cắt cỏ (Mower): Nếu bạn để zombie lại quá gần nhà, máy cắt cỏ sẽ tự động kích hoạt và tiêu diệt tất cả zombie hiện
  tại đang ở hàng đó. Chú ý là mỗi hàng chỉ có một máy cắt cỏ thôi.

<div style="text-align: center;">

![image](resources/preview/mower.png)
</div>

- Bố cục game cơ bản:

<div style="text-align: center;">

![image](resources/preview/game.png)
</div>

 

# 4. Cách chơi

Trồng cây để tấn công và phòng thủ, không cho zombie bước vào nhà bạn.<br/>
Để trồng cây, Chọn hạt giống của cây đó (bàn phím hoặc click chuột) rồi đặt vào ô muốn trồng. <br/>
Có thể đào những cây đang sử dụng bằng xẻng.<br/>
Cây sẽ bị zombie ăn mất.

## a. Các loại cây

Có 6 loại cây, lần lượt theo thứ tự có thể nhận được trong game là:

|                                                                             | Tên cây	     | Chức năng                                                                                                                    |
|-----------------------------------------------------------------------------|--------------|------------------------------------------------------------------------------------------------------------------------------|
| <img src="resources/images/Plant_sheet/peashooter/Peashooter.png" width=50>  | Peashooter	  | Mỗi lần bắn một đậu vào zombie.                                                                                              |
| <img src="resources/images/Plant_sheet/sunflower/Sunflower.png" width=44>    | Sunflower	   | Tạo thêm mặt trời để trồng cây                                                                                               |
| <img src="resources/images/Plant_sheet/walnut/Walnut.png" width=42>          | Wall-nut	    | Chịu được nhiều phát ăn của zombie để bảo vệ các cây khác.                                                                   |
| <img src="resources/images/Plant_sheet/snowpea/SnowPea.png" width=52>        | Snow Pea	    | Mỗi lần bắn một đậu băng vào zombie. Đậu băng không tăng sát thương nhưng làm chậm tốc độ di chuyển và tốc độ ăn của zombie. |
| <img src="resources/images/Plant_sheet/potatomine/Potato_Mine.png" width=56> | Potato Mine  | Tiêu diệt zombie khi chạm nó nhưng cần thời gian để kích hoạt.                                                               |
| <img src="resources/images/Plant_sheet/cherrybomb/Cherrybomb.png" width=48>  | Cherry Bomb	 | Ngay lập tức tiêu diệt tất cả zombie trong khu vực 3x3.                                                                      |

## b. Các loại zombie

- Có 5 loại zombie:

|                                         | Tên zombie 	       | Chức năng                                                                                        |
|-----------------------------------------|--------------------|--------------------------------------------------------------------------------------------------|
| ![image](resources/images/Choose_level/normal.png) | Browncoat Zombie	  | Ăn cây và não của bạn.                                                                           |
| ![image](resources/images/Choose_level/flag.png)   | Flag Zombie	       | Báo hiệu một lượng lớn zombie đang chuẩn bị ra.                                                  |
| ![image](resources/images/Choose_level/cone.png)   | Conehead Zombie	   | Trâu gấp 3 lần zombie thường                                                                     |
| ![image](resources/images/Choose_level/bucket.png) | Buckethead Zombie	 | Trâu gấp 6.5 lần zombie thường                                                                   |
| ![image](resources/images/Choose_level/door.png)   | Door Zombie	       | Trâu gấp 6.5 lần zombie thường. Không bị ảnh hưởng bởi sự làm chậm của đạn băng nếu vẫn còn cửa. |

# 5. Chiến thắng và thất bại

- Bạn sẽ chiến thắng nếu tiêu diệt hết tất cả zombie trong ván đó.

<div style="text-align: center;">

![image](resources/preview/win.png)
</div>

- Bạn sẽ thất bại nếu có một zombie nào đó đi hết sân để vào nhà bạn.

<div style="text-align: center;">

![image](resources/preview/lose.png)
</div>

---

### Về đồ họa của game:

[[Cách làm hiệu ứng từng cây, zombie trong game]](about_graphics.md)

### Về source code game:

- Folder draw:
    * khởi tạo window và renderer
    * chứa tất cả các ảnh trong game (được đánh số)
    * chứa hầu hết các câu lệnh của SDL2, SDL2_image, SDL2_ttf, sử dụng để vẽ load ảnh, chữ và vẽ lên renderer, window.
- Folder elements: Chứa tất cả các thành phần của game:
    * bullets: các loại đạn
    * button: các loại nút trong game (có thể nhấn được hoặc không)
    * Map: chứa hàm random và khởi tạo bảng 5 x 9 cho màn chơi.
    * mower: máy cắt cỏ (Cán zombie)
    * plants: chứa tất cả các loại plant (được kế thừa từ class Plants)
    * zombies: chứa tất cả các loại zombies (được kế thừa từ class Zombie)
    * elements: gom lại các loại plants và zombies.
    * elements_actions:
        + chứa hầu hết các tương tác giữa các thành phần game với nhau, và với màn chơi.
        + tạo ngẫu nhiên zombie
        + cập nhật tất cả các thay đổi của game theo từng khung hình
- Folder events: Xử lý các sự kiện trong game (sự kiện chuột, bàn phím)
    * in_game: xử lý các sự kiện khi đang trong một màn chơi
        + credit: phần giới thiệu các loại zombie & ready-set-plant
        + game: tất cả các sự kiện trong một màn chơi
        + lose: xử lý khi thua
        + main_menu: xử lý khi người chơi muốn trở lại phần chọn màn chơi
        + pause_menu: xử lý khi người chơi dừng game (nút dừng hoặc click ra ngoài game)
        + restart_menu: xử lý khi người chơi muốn chơi lại màn chơi
        + turbo: xử lý nút tăng tốc game
        + volume: xử lý nút điều chỉnh âm lượng nhạc nền và hiệu ứng.
        + win: xử lý khi chiến thắng trò chơi.
    * mouse_cursor: xử lý con trỏ chuột
    * out_game: xử lý các sự kiện ngoài màn chơi
        + choose_level: xử lý chọn màn chơi
        + quit_menu: xử lý nút quit game: tắt game
        + reset_level: xử lý nút reset level: xóa tiến trình chơi
        + start_game: xử lý nút "Click to start" để bắt đầu vào game.
        + unlock_plant: xử lý mở khóa plant mới.
- Folder level: xử lý phần lấy dữ liệu ván chơi.
- Folder music: xử lý phần âm thanh của game - SDL2_mixer
- Folder player: xử lý phần dữ liệu người chơi. Bao gồm việc lấy và thay đổi tên, tiến trình.
- game_stats.hpp: chứa tất cả thông số game
- game.hpp và game.cpp: hàm main() của trò chơi.

# Hệ Thống IoT phòng thông minh đơn giản

# Chức năng:

Hệ thống phòng thông minh sử dụng công nghệ IoT được thiết kế nhằm hỗ trợ người dùng quản lý căn phòng của ngôi nhà thông minh. Hệ thống điều khiển đèn phòng, sử dụng cảm biến độ ẩm nhiệt độ để đo các thông số và gửi thông tin cho người dùng, sử dụng động cơ servo để mở cửa. Mọi thao tác làm việc trên giao diện dashboard của node_red.

# Linh kiện sử dụng:
  * ESP32 Doitdevkit v1 : gửi , nhận và xử lý tín hiệu.
  * Đèn Led mô phỏng đèn phòng.
  * Động cơ Servo SG90 mô phỏng mở cửa.
  * Cảm biến độ ẩm nhiệt độ DHT11.

# Sơ đồ khối esp32:
![IOT_diagram](https://github.com/linhlinhto/IoT_Basic_Smartroom/blob/main/images/Smart_room_diagram.png)

# Sơ đồ khối Node-red:
![Node_red](https://github.com/linhlinhto/IoT_Basic_Smartroom/blob/main/images/Node_Red_diagram.png)

# Dash board:
  * sử dụng Dash_board của Node Red để hiển thị giao diện điều khiển.
  * Các chức năng chính có thể làm như hiển thị trên ảnh:
    - Hiển thị độ ẩm nhiệt độ của căn phòng
    - Điều khiển đèn bằng 1 thanh kéo với thang là từ 1 đến 255
    - Điều khiển cửa bằng 2 nút bấm mở cửa và đóng cửa
    - 
![Dash_board](https://github.com/linhlinhto/IoT_Basic_Smartroom/blob/main/images/Dash_board.png)

# Demo video:
https://github.com/user-attachments/assets/105eedda-4914-47ea-a9f8-aa787048668b




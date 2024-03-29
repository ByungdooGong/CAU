# 영상처리 프로젝트 정리

### 코드분석

```
import numpy as np
import cv2
import os

img = cv2.imread('images/3.jpg')  #영상 읽기
imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  #gray-scale로 변경
kernel = np.ones((5, 5), np.uint8)  #5x5배열로 1을 채움 
opening = cv2.morphologyEx(imgray, cv2.MORPH_OPEN, kernel) #원래는 이진화 먼저해야됨.
```

리스트와 배열(array)의 차이점 : 배열은 적은 메모리로 데이터를 빠르게 처리할 수 있다. 

1. 모든 원소가 같은 자료형이여야 한다.
2. 원소의 갯수를 바꿀 수 없다.

Numpy : 파이썬은 자체적으로 배열 자료형을 제공하지 않기에 수치해석용 파이썬 패키지.

- C로 구현된 내부 반복문을 사용하기 때문에 파이썬 반복문에 비해 속도가 빠르며 벡터화 연산을 이용하여 간단한 코드로도 복잡한 선형 대수 연산을 수행.
- 배열 인덱싱을 사용한 질의(Query) 기능을 이용하여 간단한 코드로도 복잡한 수식을 계산가능

np.uint8 : 0을 포함한 양의 정수(8bit까지 표현 - 0~2^8 : 256개)

morphology(모폴로지 연산)

1. erosion(침식) - 바이너리 이미지에서 흰색 오브젝트의 외곽 픽셀을 0(검은색)으로 만듬
   						- 외곽을 깍아냄
   						- 예를 들어 2x2의 1로 이루어진 필터일 경우 네개가 다 1이면 1, 아니면 0
   						- 노이즈를 없애기 위해 사용
2. dilation(팽창) - 외곽 픽셀 주변에 1(흰색) 추가
   - 외곽에 살을 붙임
   - 예를 들어 2x2의 1로 이루어진 필터일 경우 네 개중 한개라도 1이면 1로 채우고, 아니면 0
   - 노이즈를 줄이기 쪼그라든 이미지를 복원
3. opening - 침식 + 팽창, 노이즈 제거에 사용
4. closing - 팽창 + 침식, 흰색 오브젝트에 있는 작은 검은 구멍들을 메꾸는데 사용

```
oretval, othreshold = cv2.threshold(opening, 150, 255, cv2.THRESH_BINARY_INV)
```

threshold - 임계값보다 작으면 0 이된다. 그림자나 조명의 영향을 많이 받음.

adaptivethreshold - 각 픽셀에 대한 threshold를 이웃하는 픽셀값에 의해 결정. 

1. 선택된 픽셀 위치에 대해 bxb 크기의 블록을 설정한다.
2. 블록에 대한 가중 평균(weighted average)을 구한다. (블록 내의 모든 픽셀에 대한 평균을 구하는 방법, 블록 내의  픽셀들에 대한 가우시안 가중평균을 구하는 방법)
3. 앞에서 구한 평균값에서 상수를 빼면 Threshold가 정해짐.  T(x, y) = WA(x, y) - param1

//참조 https://webnautes.tistory.com/1034

otsu 이진화 - bimodal 이미지(peak가 2개 이상인)에서 사용. 1979년 otsu가 제안. 영상의 히스토그램 형태가 쌍봉형(bimodal)이라 가정했을 때 그 계곡점(valley)를 찾아서 그 점을 임계값으로 함.

- 전체분산 =  클래스 내 분산 + 클래스 간 분산

- 클래스 내 분산을 최소화하는 방식
- 전경 배경 분리

//https://j07051.tistory.com/364

findcontour - 등고선처럼 같은 값을 갖는 점들을 찾아냄. 주로 이진화 시킨후 사용. suzuki85 알고리즘 사용

- 외곽정보만 뽑아냄
- contour 정보를 넘겨줌
- canny와의 차이점 : 가장자리는 그라디언트 방향의 이미지 그라디에이션의 극한 인 점으로 계산, 윤관선은 객체 윤곽선을 목표로 하고 닫힌 커브가 필요.

```
output = cv2.connectedComponentsWithStats(othreshold, 8, cv2.CV_32S)
num_labels = output[0] #label 수
labels = output[1]
stats = output[2] #area, left, top, width, height
centroids = output[3] #가운데 좌표

temp = []
count = 0
        
for i in range(num_labels):
    temp.append(stats[i, cv2.CC_STAT_LEFT])
far_left_i = []
for i in range(num_labels):
    far_left_i.append(0)

area = lambda i:stats[i, cv2.CC_STAT_AREA]
left = lambda i:stats[i, cv2.CC_STAT_LEFT]
top = lambda i:stats[i, cv2.CC_STAT_TOP]
width = lambda i:stats[i, cv2.CC_STAT_WIDTH]  
height  = lambda i:stats[i, cv2.CC_STAT_HEIGHT]
right = lambda i:left(i) + width(i)
bottom = lambda i:top(i) + height(i)
centerX = lambda i:left(i)+(width(i)/2)
centerY = lambda i:top(i) + (height(i)/2)
```

connectedComponents 함수 -> stat정보(left, top, width, height 등) 이용
```
k = 0
far_left = 10000
while k<num_labels:
    for i in range(0, num_labels):
        if far_left > temp[i]:
            far_left = temp[i]
            far_left_i[k] = i
            
    temp[far_left_i[k]] = 10000
    far_left = 10000
    k = k + 1        

reA = []
for i in range(num_labels):
    if stats[far_left_i[i], cv2.CC_STAT_AREA] > 20:
        reA.append(far_left_i[i]);
       
p =0
```
왼쪽부터 오른쪽 순으로 라벨링 하기 위한 작업

라벨이 뒤죽박죽 되있는데 far_left_i 배열에 왼쪽에 있는 것부터 그 포인트들을 저장

각 숫자의 left, top, width, height 배열로 저장. area>20이상인 것만 검출하게 설정(오답률 줄이기위해).

```
for i in range(1, len(reA)):
    if flag > 0:
        flag -= 1
        continue
    if i+1<len(reA) and left(reA[i]) < centerX(reA[i+1]) and right(reA[i]) > centerX(reA[i+1]) and (top(reA[i]) > centerY(reA[i+1]) or
    bottom(reA[i]) < centerY(reA[i+1])):
        #나눗셈
        if i+2<len(reA) and left(reA[i]) < centerX(reA[i+2]) and right(reA[i]) > centerX(reA[i+2]) and (top(reA[i]) > centerY(reA[i+2]) or
    bottom(reA[i]) < centerY(reA[i+2])):
             subimg = threshold[min(top(reA[i+1]),top(reA[i+2])):max(bottom(reA[i+2]), bottom(reA[i+2])), left(reA[i]):right(reA[i])]
             image = cv2.rectangle(image, (left(reA[i]), min(top(reA[i+1]),top(reA[i+2]))),(right(reA[i]),max(bottom(reA[i+1]), bottom(reA[i+2]))), (0, 0, 255), 2)
             flag = 2
             
        #등호
        else:
            subimg = threshold[min(top(reA[i]), top(reA[i+1])):max(bottom(reA[i+1]),bottom(reA[i])), 
                                   min(left(reA[i]), left(reA[i+1])):max(right(reA[i+1]),right(reA[i])) ]
            image = cv2.rectangle(img, (min(left(reA[i]), left(reA[i+1])), min(top(reA[i]), top(reA[i+1]))),
                                     (max(right(reA[i+1]),right(reA[i])), max(bottom(reA[i+1]),bottom(reA[i]))), (0, 0, 255), 2)
            
            flag =  1
            
    #루트
    elif i+1<len(reA) and left(reA[i]) < centerX(reA[i+1]) and right(reA[i]) > centerX(reA[i+1]) and (top(reA[i]) < centerY(reA[i+1]) and
    bottom(reA[i]) > centerY(reA[i+1])):
        subimg = threshold[top(reA[i]):bottom(reA[i]), left(reA[i]):right(reA[i])]
        temp_img = np.zeros(threshold.shape, np.uint8)
        for y in range(threshold.shape[0]):
            for x in range(threshold.shape[1]):
                if(labels[y, x] == reA[i]):
                    temp_img[y-top(reA[i]), x-left(reA[i])] = 255
        subimg = temp_img [0:height(reA[i]), 0:width(reA[i])]
        for j in range(i+1, len(reA)):
            if left(reA[i]) < centerX(reA[j]) and right(reA[i]) > centerX(reA[j]) and (top(reA[i]) < centerY(reA[j]) and
    bottom(reA[i]) > centerY(reA[j])):
               
                count += 1
        arr[savenum] = count
        count = 0
       # continue
    else:
        subimg = threshold[top(reA[i]):bottom(reA[i]), left(reA[i]):right(reA[i])]
        #지수
        if i+1<len(reA) and width(reA[i])*height(reA[i]) > 2*(width(reA[i+1]))*(height(reA[i+1]))and centerY(reA[i+1]) < top(reA[i]):
                arr[savenum+1] = -1
```

일반적으로는 그냥 인식해도 되는데 나눗셈, 등호, 루트, 지수의 경우 component가 분리되어 있기 때문에 추가적인 처리를 하였음. 예를 들어 나눗셈은 각 점들이 작대기 안쪽에 있으면 한 묶음으로, 루트, 지수 같은 경우는 특별하여 따로 루트, 지수라는 표시를 해주는 arr배열을 생성하여 저장.

**arr배열**
- 지수 : -1
- 루트 : 루트 안에 있는 성분들 개수 count
```
 rate_h = 62.0 / subimg.shape[0] 
    rate_w = 62.0 / subimg.shape[1] 
    rate = min(rate_h, rate_w)
    shrink = cv2.resize(subimg, None, fx=rate, fy=rate, interpolation=cv2.INTER_AREA)

    white = np.zeros((62,62,1), np.uint8)
    white[:] = 255

    diff = 31 - shrink.shape[1]/2
    diff_y = 31 - shrink.shape[0]/2

    for a in range(shrink.shape[0]):
        for b in range(shrink.shape[1]):
            if shrink[a,b] > 0:
                white[a+int(diff_y),b+int(diff)] = 0
    savenum = savenum + 1 
    features.append(white)

save_arr = []
for i in range(savenum):
    save_arr.append(arr[i])
image_set={}
image_set["features"]=np.asarray(features)
```

cv2.resize : feature들을 cnn 모델 사이즈에 맞게 줄이고 

학습을 위해 데이터 보냄.
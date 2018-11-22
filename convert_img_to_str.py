# coding:utf8
import base64
import cv2

image_file = "lstm.jpg"


def main():
    # convert image format to png
    im_in = cv2.imread(image_file)
    im_out = "temp.png"
    cv2.imwrite(im_out, im_in)
    # generate base64 string
    with open(im_out, "rb") as f:
        im_str = base64.b64encode(f.read())
        print(im_str)
        output = open("out.txt", "w")
        output.write(str(im_str))
        output.close()


if __name__ == "__main__":
    main()

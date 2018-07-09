# import the necessary packages
import numpy as np
import cv2
import os
from tqdm import tqdm
import matplotlib.pyplot as plt

def show_images(images, cols = 1, titles = None):
	assert((titles is None) or (len(images) == len(titles)))
	n_images = len(images)
	if titles is None: titles = ['Image (%d)' % i for i in range(1,n_images + 1)]
	fig = plt.figure()
	for n, (image, title) in enumerate(zip(images, titles)):
		a = fig.add_subplot(cols, np.ceil(n_images/float(cols)), n + 1)
		if image.ndim == 2:
			plt.gray()
		plt.imshow(image)
		a.set_title(title)
	fig.set_size_inches(np.array(fig.get_size_inches()) * n_images)
	plt.show()

def check_color(image):
	# load the image
	#image = cv2.imread('index.png')
	#image = cv2.imread(image)
	image = cv2.resize(image, (250,250))
	# define the list of boundaries
	boundaries = [
		([31, 75, 0], [65, 160, 10]),		#P1
		([7, 0, 113], [20, 10, 230]),		#P2
		([70, 0, 118], [150, 0, 250]),		#P3
		([113, 86, 0], [230, 170, 10]),		#P4
		([75, 0, 50], [160, 255, 105]),		#P5
		([75, 100, 125], [160, 210, 255]),	#P6
		([86, 31, 4], [140, 40, 50]),		#P7
		([0, 118, 127], [10, 240, 255])		#P8
	]

	# loop over the boundaries
	output = {}#, pat = [], []
	patterns = []
	n = 1
	for (lower, upper) in boundaries:
		# create NumPy arrays from the boundaries
		lower = np.array(lower, dtype = "uint8")
		upper = np.array(upper, dtype = "uint8")

		# find the colors within the specified boundaries and apply
		# the mask
		mask = cv2.inRange(image, lower, upper)
		out_img = cv2.bitwise_and(image, image, mask = mask)
		#average_color = [out_img[:,:,i] for i in range(out_img.shape[-1])]
		#if (np.sum(average_color)/out_img.shape[-1]) > 100000:
		output[n] = out_img
		#pat.append(n)
		n += 1
		#print(output)
		# show the images
		#cv2.imshow("images", np.hstack([image, output]))
		#cv2.waitKey(0)
	for i, im in output.items():#enumerate(output):
		average_color = [im[:,:,j] for j in range(im.shape[-1])]
		if (np.sum(average_color)/im.shape[-1]) > 100000:
			patterns.append(i)
	return patterns
"""
TEST_DIR = 'Colored_patterns/Train'
for img in tqdm(os.listdir(TEST_DIR)):
	path = os.path.join(TEST_DIR, img)
	#out_img, out_pat = check_color(path)[0], check_color(path)[1]
	#show_images(out_img, titles = out_pat)
	# for i, im in enumerate(out_img):
	# 	average_color = [im[:,:,i] for i in range(im.shape[-1])]
	# 	if (np.sum(average_color)/im.shape[-1]) > 100000:
	# 		print('detected pattern is: ' + str(i+1))
	print('detected pattern(s) is/are: ' + str(check_color(path)))
"""
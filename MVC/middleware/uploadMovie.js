const multer = require('multer');
const path = require('path');

// Configure storage for videos and images
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    if (file.fieldname === 'video') {
      cb(null, 'uploads/videos'); // Save videos in 'uploads/videos'
    } else if (file.fieldname === 'moviePicture') {
      cb(null, 'uploads/images'); // Save images in 'uploads/images'
    } else {
      cb(new Error('Unexpected field type'));
    }
  },
  filename: (req, file, cb) => {
    const uniqueSuffix = `${Date.now()}-${Math.round(Math.random() * 1e9)}`;
    cb(null, `${file.fieldname}-${uniqueSuffix}${path.extname(file.originalname)}`);
  },
});

// File filter for video and image validation
const fileFilter = (req, file, cb) => {
  const videoTypes = /mp4|mov|avi|mkv/;
  const imageTypes = /jpeg|jpg|png|gif/;

  if (file.fieldname === 'video') {
    cb(null, true);
  } else if (file.fieldname === 'moviePicture') {
    cb(null, true);
  } else {
    cb(new Error(`Unexpected field type: ${file.fieldname}`));
  }
};

// Configure multer
const upload = multer({
  storage,
  fileFilter,
  limits: { fileSize: 100 * 1024 * 1024 }, // 100MB limit
}).fields([
  { name: 'video', maxCount: 1 },
  { name: 'moviePicture', maxCount: 1 },
]);

module.exports = upload;

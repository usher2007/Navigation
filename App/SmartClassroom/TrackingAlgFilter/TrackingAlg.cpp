#include "stdafx.h"
#include "TrackingAlg.h"

TrackingAlg::TrackingAlg()
{
	frameIndex = 0;
	bShowTrackingResult = FALSE;
}

int TrackingAlg::Update(cv::Mat& frame)
{
	if(frame.empty())
		return -1;

	int res;
	if(frameIndex == 0)
	{
		bgSubtractor(frame, gbmForeground, TrackingConfig::GBM_LEARNING_RATE);
		cv::threshold(gbmForeground, gbmForeground, TrackingConfig::FG_LOW_THRESH, TrackingConfig::FG_UP_THRESH, THRESH_BINARY);
		frameIndex ++;
		return 0;
	}

	if(frameIndex%TrackingConfig::TRACK_INTERVAL != 0)
	{
		frameIndex++;
		if(bShowTrackingResult)
		{
			drawAuxInfo(frame);
			personManager.DrawPersons(frame);
		}
		return 0;
	}

	bgSubtractor(frame, gbmForeground, TrackingConfig::GBM_LEARNING_RATE);
	cv::threshold(gbmForeground, gbmForeground, TrackingConfig::FG_LOW_THRESH, TrackingConfig::FG_UP_THRESH, THRESH_BINARY);

	gbmForeground = gbmForeground/255;
	std::vector<cv::Point2f> baryCenters;
	res = CalcImageBaryCenters(gbmForeground, baryCenters);
	if(res >= 0)
	{
		std::vector<cv::Point2f>::iterator centerIter;
		personManager.ResetAllPersonStatus();

		for(centerIter=baryCenters.begin(); centerIter!=baryCenters.end(); ++centerIter)
		{
			personManager.ProcessBaryCenter(*centerIter);

		}
		personManager.Update();

	}
	if(bShowTrackingResult)
	{
		drawAuxInfo(frame);

		personManager.DrawPersons(frame);
	}
	frameIndex ++;
}

int TrackingAlg::CalcImageBaryCenters(const cv::Mat& img, std::vector<cv::Point2f>& baryCenters)
{
	cv::Mat doubleImg = cv::Mat(img.rows, img.cols, CV_64F);
	img.convertTo(doubleImg, CV_64F);
	cv::Mat leftOnes = cv::Mat::ones(1, img.rows, CV_64F);
	cv::Mat rightOnes = cv::Mat::ones(img.cols, 1, CV_64F);
	cv::Mat foreHistByCol = leftOnes*doubleImg;
	int maxCol = foreHistByCol.cols;
	cv::Mat tmpRes = foreHistByCol*rightOnes;
	double avgForeHist = tmpRes.at<double>(0,0)/foreHistByCol.cols;
	std::vector<int> foreCandidates;
	for(int j=0; j<maxCol; j++)
	{
		if(foreHistByCol.at<double>(0,j) > TrackingConfig::FG_HIST_THRESH*avgForeHist)
		{
			foreCandidates.push_back(j);
		}
	}
	if(foreCandidates.empty())
	{
		return -1;
	}
	std::vector<int>::iterator it = foreCandidates.begin();
	int lastIndex = *it;
	int continueNum = 0;
	double sumWeight = 0;
	double eachColSum = 0;
	for(; it != foreCandidates.end(); it ++)
	{
		if((*it) - lastIndex < TrackingConfig::LEAST_HUMAN_GAP && it != foreCandidates.end()-1)
		{
			continueNum++;
			sumWeight += (*it)*foreHistByCol.at<double>(0,(*it))/1000;
			eachColSum += foreHistByCol.at<double>(0,(*it))/1000;
		}
		else
		{
			if(continueNum > TrackingConfig::HUMAN_WIDTH)
			{
				int personWidth = (*(it-1))-(*(it-continueNum));
				cv::Mat person = cv::Mat(doubleImg,cv::Rect(*(it-continueNum),0,personWidth,doubleImg.rows));
				cv::Mat personOnes = cv::Mat::ones(doubleImg.rows,personWidth,CV_64F);
				cv::Point2f center;
				center.x = sumWeight/eachColSum;
				double sumY = 0, sumWeightY = 0;
				for(int xPos=0; xPos<person.rows; xPos++)
					for(int yPos=0; yPos<person.cols; yPos++)
					{
						sumY += person.at<double>(xPos,yPos);
						sumWeightY += xPos*person.at<double>(xPos,yPos);
					}
					center.y = sumWeightY/sumY;
					double centerWeight = eachColSum/continueNum*1000;
					if(centerWeight > TrackingConfig::CENTER_WEIGHT_THRESH)
						baryCenters.push_back(center);
			}
			continueNum = 0;
			sumWeight = 0;
			eachColSum = 0;
		}
		lastIndex = (*it);
	}
	return 0;
}

std::vector<cv::Point2f> TrackingAlg::GetTrackedPerson()
{
	return personManager.GetTrackedPersons();
}

HRESULT TrackingAlg::SetShowTracking(BOOL bShowTracking)
{
	this->bShowTrackingResult = bShowTracking;
	return S_OK;
}

HRESULT TrackingAlg::CacheAndShowBZoneVertex(int xPix, int yPix)
{
	if(xPix >=0 && yPix >= 0)
	{
		yPix = 576 - yPix;
		cv::Point2f vertex(xPix, yPix);
		m_vertexMutex.Lock();
		m_cachedBZoneVertexes.push_back(vertex);
		m_vertexMutex.Unlock();
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT TrackingAlg::EraseCachedVertexes()
{
	m_vertexMutex.Lock();
	m_cachedBZoneVertexes.clear();
	m_vertexMutex.Unlock();
	return S_OK;
}

HRESULT TrackingAlg::AddBZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	if(x1>=0 && y1>=0 && x2>=0 && y2>=0 && x3>=0 && y3>=0 && x4>=0 && y4>=0 )
	{
		y1 = 576 - y1;
		y2 = 576 - y2;
		y3 = 576 - y3;
		y4 = 576 - y4;
		BlindZoneShape bZoneShape;
		bZoneShape.x[0] = x1;
		bZoneShape.x[1] = x2;
		bZoneShape.x[2] = x3;
		bZoneShape.x[3] = x4;
		bZoneShape.y[0] = y1;
		bZoneShape.y[1] = y2;
		bZoneShape.y[2] = y3;
		bZoneShape.y[3] = y4;

		m_bzoneShapeMutex.Lock();
		if(containSameBZoneShape(bZoneShape))
		{
			return S_OK;
		}
		else
		{
			m_bZoneShapeList.push_back(bZoneShape);
		}
		m_bzoneShapeMutex.Unlock();

		cv::Point2f vertex1(y1, x1);
		cv::Point2f vertex2(y2, x2);
		cv::Point2f vertex3(y3, x3);
		cv::Point2f vertex4(y4, x4);

		cvBZoneVertexList bzoneVertexes;
		bzoneVertexes.push_back(vertex1);
		bzoneVertexes.push_back(vertex2);
		bzoneVertexes.push_back(vertex3);
		bzoneVertexes.push_back(vertex4);

		cv::Mat bzone = cv::Mat::ones(576, 720, CV_8UC3)*255;
		for(int i=0; i<576; i++)
			for(int j=0; j<720; j++)
			{
				cv::Point pt(i, j);
				
				if(cv::pointPolygonTest(bzoneVertexes, pt, false) >= 0)
				{
					cv::Vec3b v(0, 0, 0);
					bzone.at<Vec3b>(i, j) = v;
				}
				else
				{
					cv::Vec3b v(1, 1, 1);
					bzone.at<Vec3b>(i, j) = v;
				}
			}

		/*cvNamedWindow("Debug");
		imshow("Debug", bzone);
		cvWaitKey();*/
		m_bzoneMutex.Lock();
		m_blindZoneList.push_back(bzone);
		m_bzoneMutex.Unlock();
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT TrackingAlg::ClearBZones()
{
	m_bzoneMutex.Lock();
	m_blindZoneList.clear();
	m_bzoneMutex.Unlock();

	m_bzoneShapeMutex.Lock();
	m_bZoneShapeList.clear();
	m_bzoneShapeMutex.Unlock();

	return S_OK;
}

HRESULT TrackingAlg::DrawBZoneVertexes(cv::Mat &frame)
{
	if(drawBZoneVertexes(frame) == 0)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT TrackingAlg::DrawBZones(cv::Mat &frame)
{
	if(drawBZones(frame) == 0)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

bool TrackingAlg::containSameBZoneShape(const BlindZoneShape& bZoneShape)
{
	if(m_bZoneShapeList.empty())
	{
		return false;
	}
	BZoneShapeIter bZoneShapeIter;
	bool res = true;
	for(bZoneShapeIter=m_bZoneShapeList.begin(); bZoneShapeIter!=m_bZoneShapeList.end(); ++bZoneShapeIter)
	{
		BOOL currentEq = true;
		for(int i=0; i<4; i++)
		{
			if(bZoneShapeIter->x[i] != bZoneShape.x[i] || bZoneShapeIter->y[i] != bZoneShape.y[i])
			{
				currentEq = false;
				break;
			}
		}
		if(currentEq)
		{
			return true;
		}
	}
	return false;

}

int TrackingAlg::drawAuxInfo( cv::Mat& frame )
{
	cv::rectangle(frame, TrackingConfig::BEGIN_TRACKING_AREA, cv::Scalar(255,0,0), 2);
	cv::rectangle(frame, TrackingConfig::STOP_TRACKING_AREA, cv::Scalar(0,0,255), 2);
	cv::rectangle(frame, TrackingConfig::PAD_AREA_1, cv::Scalar(0,255,0), 3);
	cv::rectangle(frame, TrackingConfig::PAD_AREA_2, cv::Scalar(0,255,0), 3);
	cv::rectangle(frame, TrackingConfig::PAD_AREA_3, cv::Scalar(0,255,0), 3);
	cv::rectangle(frame, TrackingConfig::PAD_AREA_4, cv::Scalar(0,255,0), 3);

	return 0;
}

int TrackingAlg::drawBZoneVertexes(cv::Mat &frame)
{
	if(frame.empty())
		return -1;

	m_vertexMutex.Lock();
	if(!(m_cachedBZoneVertexes.empty()))
	{
		cvBZoneVertexIter vertexIter;
		for(vertexIter=m_cachedBZoneVertexes.begin(); vertexIter!=m_cachedBZoneVertexes.end(); ++vertexIter)
		{
			cv::circle(frame, *vertexIter,4, cv::Scalar(0, 0, 255), -1);
		}
	}
	m_vertexMutex.Unlock();
	return 0;
}

int TrackingAlg::drawBZones(cv::Mat &frame)
{
	if(frame.empty())
		return -1;

	if(!(m_blindZoneList.empty()))
	{
		m_bzoneMutex.Lock();
		cvBlindZoneIter bzoneIter;
		for(bzoneIter=m_blindZoneList.begin(); bzoneIter!=m_blindZoneList.end(); ++bzoneIter)
		{
			frame = frame.mul(*bzoneIter);
		}
		m_bzoneMutex.Unlock();
	}
	return 0;
}
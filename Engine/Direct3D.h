#pragma once

class Direct3D {
	public:
		Direct3D(int width, int height);
		~Direct3D();

		bool Init();
		bool Frame();


	private:
		int m_Width;
		int m_Height;
};


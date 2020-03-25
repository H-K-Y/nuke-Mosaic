


static const char * const HELP = "MaSaiKe";
static const char * const CLASS = "MaSaiKe";


#include "DDImage\Iop.h"
#include "DDImage\Row.h"
#include "DDImage\Knobs.h"
#include "DDImage\Tile.h"
#include "DDImage\NukeWrapper.h"



using namespace DD::Image;


class MaSaiKe : public Iop
{
public:
	double _size;

	MaSaiKe(Node * node) :Iop(node) { _size = 5.0; }
	~MaSaiKe() {}

	void _validate(bool for_real);
	void _request(int x, int y, int r, int t, ChannelMask channels, int count);
	void engine(int y, int l, int r, ChannelMask channels, Row& row);


	virtual void knobs(Knob_Callback);
	const char* Class() const { return CLASS; }
	const char* node_help() const { return HELP; }
	static const Iop::Description d;


};


void MaSaiKe::_validate(bool for_real)
{
	copy_info();
	info_.pad(_size);

}



void MaSaiKe::_request(int x, int y, int r, int t, ChannelMask channels, int count)
{

	input(0)->request(x - (_size + 1.0) * 2, y - (_size + 1.0) * 2, r + (_size + 1.0) * 2, t + (_size + 1.0) * 2, channels, count);
}


void MaSaiKe::engine(int y, int x, int r, ChannelMask channels, Row& row)
{


	Tile tile(input0(), x - _size * 2, y - _size * 2, r + _size * 2, y + _size * 2, channels);
	if (aborted()) {
		std::cerr << "Aborted!"<<std::endl;
		return;
	}



	foreach(z, channels) {
		float* out = row.writable(z);


		for (int i = x; i < r; i++)
		{
			int x_index = i / _size;
			int y_index = y / _size;



			float value = 0.0;

			for (int x_offset = 0 ; x_index * _size+x_offset < (x_index+1)*_size ; x_offset++ )
			{
				for (int y_offset = 0; y_index*_size + y_offset < (y_index+1)*_size; y_offset++ )
				{
					value += tile[z][tile.clampy(y_index*_size + y_offset)][tile.clampx(x_index*_size + x_offset)];

				
				}

			}

			

			out[i] = value/(_size*_size);
		}


	}
}





void MaSaiKe::knobs(Knob_Callback f)
{
	Double_knob(f, &_size, "size");
	Tooltip(f, "size");

}

static Iop* build(Node* node)
{
	return new MaSaiKe(node);
}
const Iop::Description MaSaiKe::d(CLASS, "MaSaiKe", build);


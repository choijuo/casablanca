
#include <stdio.h>
#include <time.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>


using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;





void HttpsUploader::RequestGetAuthKey(int i){
	string getURL="https://123,123,12,123:12345/qw/qwe/qwer";

    try{
        http_client_config config;//http_client_config 객체 생성
        


		config.set_validate_certificates(false);//인증서 무시하는 옵션

        //http_client client(U("https://123,123,12,123:12345/qw/qwe/qwer"),config);
		http_client client(U(getURL),config);//client 객체에 config 
        http_request request(methods::GET); //get 전송
        request.headers().add("CID",cid); // request header 설정 부분
        request.headers().add("Authorization",authorization);


        client.request(request).then([&](http_response res){ //요청 응답 처리부분
            //auto resCode=res.status_code();
            //if(resCode==200){
                frDEBUG(("Request to server"));
            //}

            res.extract_json(true).then([&](json::value v){//응답부분 json
                    if(v.is_null()){return;}
                    string jsonVal=v.serialize();//json 전체를 jsonVal에 담음
                    auto resCode=v.at(U("response_code")).as_integer();//응답 코드 받기
                    if(resCode==400){
                        frDEBUG(("response_code : %d,BAD REQUEST,check header value\n",resCode));
                        return;
                    }else if(resCode==401){
                        frDEBUG(("response_code : %d,Unauthorized,Wrong cid or Authorization value in Request Header\n",resCode));
                        return;
                    }else if(resCode==404){
                        frDEBUG(("response_code : %d,Not Found,Wrong URL\n",resCode));
                        return;
                    }else if(resCode==500){
                        frDEBUG(("response_code : %d,Internal Server Error\n",resCode));
                        return;
                    }else if(resCode==0){
                        frDEBUG(("response_code : %d,Can't connect to server\n",resCode));
                        return;
    
                    }else if(resCode==200){
                        frDEBUG(("response_code : %d,Request Correct",resCode));
                        auto now=v.at(U("a")).as_integer();
                        auto exp=v.at(U("b")).as_integer();
                        auto r_id=v.at(("c")).as_string();
                        auto ac_tkn=v.at(("d")).as_string();
						//json 응답에 대해 key a,b,c,d 의 대한 value를 저장한것




                    }
                    //resCode_val=resCode;

            }).wait(); //처리될때까지 기다리는것
    }).wait();
        reqAuthCnt=0;
    }catch(const http_exception &e){

	//예외처리부분

    } 

    //m_fileLock.UnLock();

}
int HttpsUploader::RequestPostData(string a,int b,const char *testData){


    
    string data;
    int response_code=0;
    data=testData;


    string resData;

	string val1="";
	string val2="";
    
	//request Body 사용할때
	//ex)
	//{
	//	"r1" : "124124",
	//	"r2" : 3,
	//	"r3" : "51251121515"
	//	}
    web::json::value json_v ;//json 형식의 requestbody 선언
    json_v["r1"] = web::json::value::string(a);
    json_v["r2"] = web::json::value::number(b);
    json_v["r3"] = web::json::value::string(testData);

    http_client_config config;
    config.set_validate_certificates(false);
    http_client client(U(m_postURL),config);

    http_request request(methods::POST);//POST 전송 설정

	//request header 사용
    request.headers().add("val1",val1);
    request.headers().add("val2",val2);
    request.headers().add("Content-Type","application/json");

	//위에서 설정한 request body 설정
	request.set_body(json_v);


    client.request(request).then([&](http_response res){
            auto resCode=res.status_code();
            if(resCode==400){
                    frDEBUG(("response_code : %d,BAD REQUEST,check header value\n",resCode));
                }else if(resCode==401){
                    frDEBUG(("response_code : %d,Unauthorized,Wrong cid or Authorization value in Request Header\n",resCode));
                }else if(resCode==404){
                    frDEBUG(("response_code : %d,Not Found,API Wrong URL \n",resCode));
                }else if(resCode==500){
                    frDEBUG(("response_code : %d,Internal Server Error\n",resCode));
                }else if(resCode==200){
                    frDEBUG(("response_code : %d,Correct\n",resCode));
                }else if(resCode==0){
                    frDEBUG(("response_code : %d,Can't connect to Server\n",resCode));
                }
            response_code=resCode;
            string st=res.to_string();
          

    }).wait();

    return response_code;
}



int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	
	return 0;
}

package org.FXUtils;


import java.io.File;
import java.io.IOException;

import com.camellia.starfall.star;


public class FileUtils {

	/**
	 * 创建一个doc目录下面的文件
	 * @param dir		比如/infor/
	 * @param fileName 	比如"infor.txt"
	 * @return
	 */
	public static boolean createFile(String dir, String fileName) {
		
		String dataRoot = "/data/data/";
		String t1 = star.getContext().getPackageName();
		
		String packagePath = t1;
		File dirFile = new File(dataRoot+packagePath+"/files/"+dir);
		dirFile.mkdirs();
		File file = new File(dirFile.getAbsolutePath()+"/"+fileName);
		try {
			//这里内部已经判断了文件是否存在
			//不用考虑重复创建的问题
			file.createNewFile();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("创建文件异常");
			return false;
		}
		System.out.println("FileUtils --> createFile : "+"[" + file +"]");
		return true;
	}
	
	/**
	 * 在已存在的指定目录下创建新目录
	 * @param parentDir		比如/doc/
	 * @param filterName 	比如infor
	 * @return 目录存在与否
	 */
	public static boolean createFolder(String parentDir, String filterName)
	{
		File dirFile = new File(parentDir+filterName);
		return (dirFile.mkdirs() || dirFile.isDirectory());
	}
	
	public static Boolean isExist(String fileFullPath)
	{
		File f = new File(fileFullPath);
		
		return f.exists();
	}
}

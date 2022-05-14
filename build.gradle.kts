import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
	id("java")
	id("maven-publish")
}

group = "me.tagavari"
version = "1.0.4"

repositories {
	mavenCentral()
}

dependencies {
	testImplementation("org.junit.jupiter:junit-jupiter-api:5.8.2")
	testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.8.2")
}

tasks.getByName<Test>("test") {
	useJUnitPlatform()
}

val cmakeSourceDir = File(projectDir, "native")
val cmakeOutputDir = File(buildDir, "cmake")
val headerOutputDir = File(buildDir, "header")
val nativeLibOutputDir = File(buildDir, "native")

//Gets a string in the format of 'os-arch' for this machine
val nativeString: String
	get() = project.properties["natives"] as String? ?: run {
		val osName = DefaultNativePlatform.getCurrentOperatingSystem().let { currentOS ->
			when {
				currentOS.isLinux -> "linux"
				currentOS.isWindows -> "windows"
				currentOS.isMacOsX -> "macos"
				else -> {
					val osFamilyName = currentOS.toFamilyName()
					logger.warn("Using unsupported OS $osFamilyName")
					osFamilyName
				}
			}
		}
		
		val osArch = DefaultNativePlatform.getCurrentArchitecture().let { currentArch ->
			when(currentArch.name) {
				"x86_64", "x64", "x86-64" -> "x86_64"
				"arm-v8", "armv8", "arm64" -> "arm64"
				else -> {
					val osArchName = currentArch.name
					logger.warn("Using unsupported architecture $osArchName")
					osArchName
				}
			}
		}
		
		"$osName-$osArch"
	}

tasks.jar {
	archiveClassifier.set(nativeString)
}

tasks.compileJava {
	if(JavaVersion.current() >= JavaVersion.VERSION_1_10) {
		//Compile for Java 8
		options.release.set(8)
	}
	
	//Generate native headers when compiling Java
	headerOutputDir.mkdir()
	options.compilerArgs.addAll(listOf("-h", headerOutputDir.path))
}

task<Exec>("prepareCMake") {
	group = BasePlugin.BUILD_GROUP
	description = "Generates build instructions using CMake."
	
	dependsOn(tasks.compileJava)
	
	//Get the build directory
	workingDir = cmakeOutputDir
	cmakeOutputDir.mkdir()
	nativeLibOutputDir.mkdir()
	
	//Specify inputs and outputs
	inputs.file(File(cmakeSourceDir, "CMakeLists.txt"))
	outputs.dir(cmakeOutputDir)
	
	//Run CMake
	commandLine(
		"cmake",
		cmakeSourceDir,
		"-DJNI_GENERATED_DIR=$headerOutputDir",
		"-DJAVA_HOME=${org.gradle.internal.jvm.Jvm.current().javaHome}",
		"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=$nativeLibOutputDir",
		"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$nativeLibOutputDir"
	)
}

task<Exec>("buildCMake") {
	group = BasePlugin.BUILD_GROUP
	description = "Builds native libraries using CMake."
	
	dependsOn("prepareCMake")
	
	workingDir = cmakeOutputDir
	
	inputs.dir(cmakeOutputDir)
	outputs.dir(nativeLibOutputDir)
	
	//Run CMake
	commandLine(
		"cmake",
		"--build",
		cmakeOutputDir,
		"--config",
		"Release"
	)
}

tasks.processResources {
	dependsOn("buildCMake")
	from(nativeLibOutputDir)
}

publishing {
	repositories {
		maven {
			name = "GitHubPackages"
			url = uri("https://maven.pkg.github.com/tagavari/libwebp4j")
			credentials {
				username = project.properties["gpr.user"] as String? ?: System.getenv("GITHUB_ACTOR")
				password = project.properties["gpr.key"] as String? ?: System.getenv("GITHUB_TOKEN")
			}
		}
	}
	
	publications {
		create<MavenPublication>("gpr") {
			from(components["java"])
			
			artifactId = "libwebp4j-$nativeString"
			
			pom {
				name.set("libwebp4j")
				description.set("A simple JNI wrapper for libwebp")
				url.set("https://github.com/tagavari/libwebp4j")
				
				licenses {
					license {
						name.set("The Apache License, Version 2.0")
						url.set("https://www.apache.org/licenses/LICENSE-2.0.txt")
					}
				}
				
				developers {
					developer {
						id.set("tagavari")
						name.set("Cole Feuer")
						email.set("hello@tagavari.me")
					}
				}
			}
		}
	}
}